import subprocess
import sys
import os
import platform
import argparse

MINISHELL_EXEC = "./minishell"

# Liste des tests avec nom et commandes à envoyer
tests = [
    ("Simple heredoc", "cat << END\nHello World\nEND\nexit\n"),
    ("Empty heredoc", "cat << END\n\nEND\nexit\n"),
    ("Multiple heredocs", "cat << END1\nFirst\nEND1\ncat << END2\nSecond\nEND2\nexit\n"),
    ("Heredoc with special characters", "cat << END\n$USER\nEND\nexit\n"),
    ("Heredoc with multiple lines", "cat << END\nLine1\nLine2\nLine3\nEND\nexit\n"),
    ("Heredoc with unclosed delimiter (should fail)", "cat << END\nNo end delimiter here\nexit\n"),
    ("Heredoc with escaped delimiter", "cat << END\nHello \\$USER\nEND\nexit\n"),
    ("Heredoc with variable expansion", "cat << END\nUser: $USER\nEND\nexit\n"),
    ("Heredoc with command substitution", "cat << END\nToday is $(date +%Y-%m-%d)\nEND\nexit\n"),
    ("Heredoc with pipe", "cat << END | grep World\nHello World\nEND\nexit\n"),
    ("Heredoc with multiple commands", "cat << END | grep Hello | wc -l\nHello World\nEND\nexit\n"),
    ("Heredoc with redirection to file", "cat << END > heredoc_output.txt\nSaved text\nEND\nexit\ncat heredoc_output.txt\nexit\n"),
    ("Heredoc with single quoted delimiter", "cat << 'END'\n$USER and $(echo test)\nEND\nexit\n"),
    ("Heredoc with double quoted delimiter", "cat << \"END\"\n$USER and $(echo test)\nEND\nexit\n"),
]

def run_test(name, commands, errors_only):
    # Lance le minishell en sous-processus avec pipe stdin/stdout
    proc = subprocess.Popen(
        [MINISHELL_EXEC],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )

    try:
        output, _ = proc.communicate(commands, timeout=10)
    except subprocess.TimeoutExpired:
        proc.kill()
        output, _ = proc.communicate()
        if not errors_only:
            print(f"\033[31m❌ Test '{name}' timeout ❌\033[0m")
        return False, output, True

    exit_ok = proc.returncode == 0
    leaks_detected = False

    # Sur macOS, lance leaks
    if platform.system() == "Darwin":
        leaks_cmd = ["leaks", str(proc.pid)]
        leaks_proc = subprocess.run(leaks_cmd, capture_output=True, text=True)
        leaks_output = leaks_proc.stdout
        if leaks_proc.returncode != 0:
            leaks_detected = True
    else:
        leaks_output = None

    # Si on affiche tout ou si erreur/leaks détectés
    if (not errors_only) or (not exit_ok) or leaks_detected:
        print(f"\033[36mℹ️ Running test: {name}\033[0m")
        print("Output:")
        print(output)
        if leaks_output:
            print("Leaks check output:")
            print(leaks_output)
        if exit_ok and not leaks_detected:
            print(f"\033[32m✅ Test passed! No leaks detected. ✅\033[0m")
        else:
            if not exit_ok:
                print(f"\033[31m❌ Test failed with exit code {proc.returncode} ❌\033[0m")
            if leaks_detected:
                print(f"\033[31m⚠️ Leaks detected or leaks command failed ⚠️\033[0m")
        print("-" * 50)

    return exit_ok, output, leaks_detected

def main():
    parser = argparse.ArgumentParser(description="Test minishell with heredoc commands and leaks check.")
    parser.add_argument("-e", "--errors-only", action="store_true", help="Show only tests with errors or leaks.")
    args = parser.parse_args()

    if not os.path.isfile(MINISHELL_EXEC) or not os.access(MINISHELL_EXEC, os.X_OK):
        print(f"\033[31mError: '{MINISHELL_EXEC}' executable not found or not executable.\033[0m")
        sys.exit(1)

    all_ok = True
    for name, cmds in tests:
        exit_ok, _, leaks_detected = run_test(name, cmds, args.errors_only)
        if not exit_ok or leaks_detected:
            all_ok = False

    if all_ok:
        print(f"\033[32mAll tests passed with no leaks detected.\033[0m")
    else:
        print(f"\033[31mSome tests failed or leaks detected.\033[0m")

if __name__ == "__main__":
    main()

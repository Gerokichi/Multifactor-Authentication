import subprocess
import os

# --- Настройки ---
EXE_PATH = "./zadanie2.exe"  # Если у тебя Linux/Mac, поменяй на "./zadanie2"
INFO_FILE = "info.txt"
CSV_FILE = "hesla.csv"


def get_db_state():
    db = {}
    if not os.path.exists(CSV_FILE):
        return db
    try:
        with open(CSV_FILE, 'r', encoding='utf-8') as f:
            for line in f:
                parts = line.strip().split(':')
                if len(parts) >= 3:
                    db[parts[0]] = parts[2].split(',')
    except:
        pass
    return db


def run_c_prog(u, p, k):
    try:
        proc = subprocess.Popen(
            [EXE_PATH],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )
        out, _ = proc.communicate(input=f"{u}\n{p}\n{k}\n", timeout=2)
        out_lower = out.lower()

        # Ищем только нужные слова в сыром выводе
        if "ok" in out_lower: return "ok"
        if "chyba" in out_lower: return "chyba"
        return "other"
    except Exception:
        return "error"


def main():
    if not os.path.exists(EXE_PATH):
        print(f"[!] Файл {EXE_PATH} не найден!")
        return

    users_to_test = []
    if os.path.exists(INFO_FILE):
        with open(INFO_FILE, 'r', encoding='utf-8') as f:
            for line in f:
                if ':' in line:
                    users_to_test.append(line.strip().split(':'))

    # Жесткие размеры колонок
    W_USER = 16
    W_TYPE = 12
    W_OUT = 6
    W_STAT = 8
    W_KEY = 8

    sep = "+" + "-" * (W_USER + 2) + "+" + "-" * (W_TYPE + 2) + "+" + "-" * (W_OUT + 2) + "+" + "-" * (
                W_STAT + 2) + "+" + "-" * (W_KEY + 2) + "+"
    header = f"| {'USER':<{W_USER}} | {'TEST':<{W_TYPE}} | {'OUT':<{W_OUT}} | {'STATUS':<{W_STAT}} | {'KEY':<{W_KEY}} |"

    print("\n" + sep)
    print(header)
    print(sep)

    for user, password in users_to_test:
        db = get_db_state()
        keys = db.get(user, [])
        current_key = keys[0] if keys and keys[0] else None

        if not current_key:
            print(
                f"| {user:<{W_USER}} | {'NO KEYS':<{W_TYPE}} | {'---':<{W_OUT}} | {'SKIP':<{W_STAT}} | {'---':<{W_KEY}} |")
            continue

        # --- ТЕСТ 1: ПРАВИЛЬНЫЙ ВХОД ---
        out_ok = run_c_prog(user, password, current_key)
        db_after = get_db_state()
        was_deleted = current_key not in db_after.get(user, [])

        stat_ok = "[PASS]" if out_ok == "ok" else "[FAIL]"
        key_status = "DELETED" if was_deleted else "STAYED"

        print(
            f"| {user:<{W_USER}} | {'Valid Login':<{W_TYPE}} | {out_ok.upper():<{W_OUT}} | {stat_ok:<{W_STAT}} | {key_status:<{W_KEY}} |")

        # --- ТЕСТ 2: НЕВЕРНЫЙ ПАРОЛЬ ---
        out_fail = run_c_prog(user, "bad_password", current_key)
        stat_fail = "[PASS]" if out_fail == "chyba" else "[FAIL]"

        print(
            f"| {'':<{W_USER}} | {'Wrong Pass':<{W_TYPE}} | {out_fail.upper():<{W_OUT}} | {stat_fail:<{W_STAT}} | {'---':<{W_KEY}} |")
        print(sep)


if __name__ == "__main__":
    main()
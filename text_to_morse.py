import csv

def load_morse_alphabet() -> dict:
    with open("morse_alphabet.csv", "r") as file:
        reader = csv.reader(file)
        return {row[0]: row[1] for row in reader}

def text_to_morse(text: str, should_print = False) -> str:
    morse_alphabet = load_morse_alphabet()

    morse = " ".join(morse_alphabet.get(char.upper(), "") for char in text)
    if should_print:
        print(morse)
    return morse


if __name__ == "__main__":
    text_to_morse("Hello, World!", should_print=True)


import google.generativeai as genai
import os
import random
import string

# Konfiguriere die API mit deinem Schlüssel
# Stelle sicher, dass GEMINI_API_KEY als Umgebungsvariable gesetzt ist
genai.configure(api_key=os.environ["GEMINI_API_KEY"])

def generate_random_string(length=10):
    """Generiert einen zufälligen String aus Buchstaben und Ziffern."""
    characters = string.ascii_letters + string.digits
    return ''.join(random.choice(characters) for i in range(length))

def generate_random_email_prefix():
    """Generiert ein zufälliges E-Mail-Präfix."""
    return f"{generate_random_string(8).lower()}_{generate_random_string(5).lower()}"

def generate_random_password(length=12):
    """Generiert ein zufälliges, starkes Passwort."""
    all_characters = string.ascii_letters + string.digits + string.punctuation
    password = [
        random.choice(string.ascii_lowercase),
        random.choice(string.ascii_uppercase),
        random.choice(string.digits),
        random.choice(string.punctuation)
    ]
    password.extend(random.choice(all_characters) for _ in range(length - 4))
    random.shuffle(password)
    return "".join(password)

def generate_random_person_data(model):
    """Generiert zufällige Personendaten mit Gemini."""
    prompt = "Generiere einen zufälligen, realistischen englischen Vor- und Nachnamen, ein Geburtsdatum (Format YYYY-MM-DD) und eine Adresse (Straße, Stadt, Postleitzahl, Land). Trenne die Daten durch Kommas."
    response = model.generate_content(prompt)
    return response.text.strip()

try:
    model = genai.GenerativeModel('gemini-pro')

    # Zufällige Personendaten generieren
    person_data = generate_random_person_data(model)
    print(f"Generierte Personendaten: {person_data}")

    # E-Mail-Präfix generieren
    email_prefix = generate_random_email_prefix()
    print(f"Generiertes E-Mail-Präfix: {email_prefix}")

    # Passwort generieren
    password = generate_random_password()
    print(f"Generiertes Passwort: {password}")

except Exception as e:
    print(f"Fehler bei der Generierung: {e}")


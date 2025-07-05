import google.generativeai as genai
import os

# Configure the API key
genai.configure(api_key=os.environ["GEMINI_API_KEY"])

# List available models
for m in genai.list_models():
  if "generateContent" in m.supported_generation_methods:
    print(m.name)

# Example: Generate content
model = genai.GenerativeModel('gemini-pro') # Or 'gemini-1.5-flash', 'gemini-1.5-pro', etc.
response = model.generate_content("Tell me a fun fact about the universe.")
print(response.text)

import os

# document response
print("Content-Type: text/plain")
print("X_TEST00: test")
print("X_TEST01: test")
print("X_TEST02: test")
print("")
print("Hello, world.")
print("This is body of CGI.")

for name, value in os.environ.items():
    print("{0}: {1}".format(name, value))

# local redirect response
# print("Location: /you/me/they/are")

# client redirect response with document
# print("Location: /you/me/they/are")
# print("Status: 200")
# print("Hello, Python CGI")

import os

# document response
print("conTent-tyPE:text/plain")
print("X_TEST00:test")
print("X_TEST01:test")
print("X_TEST02:test")
print("")
print("Hello, world.")
print("This is body of CGI.")

for name, value in os.environ.items():
    print("{0}: {1}".format(name, value))

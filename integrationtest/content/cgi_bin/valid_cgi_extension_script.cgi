import os

# document response
print("Content-Type:text/plain")
print("")
for name, value in os.environ.items():
    print("{0}: {1}".format(name, value))
# Local redirect response
print "Location:/\n";

# Client redirect response
print "Location:https://example.com\n";
print "Status:302 Found\n";
print "Content-Type:text/plain\n";
print "\n";
print "This is my CGI Client Redirect Response with Document.";

use strict;
use warnings;

print "Content-type: text/plain; charset=utf-8\n";
print "\n";

if ( $ENV{REQUEST_METHOD} eq "GET" ) {
    print "use 'post_cgi.html' please.";
}
elsif ( $ENV{REQUEST_METHOD} eq "POST" ) {
    read( STDIN, my $data, $ENV{CONTENT_LENGTH} );
    print $data;
}
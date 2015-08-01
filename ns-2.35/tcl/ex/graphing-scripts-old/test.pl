#! /usr/bin/perl
use strict;
use warnings;
print $_ 
my @array = (1 .. 50);
foreach my $elem (@array)
{
 if($elem % 10 == 0)
 {
     next
}
  print "$elem\t";
}

print "\nForeach is over!\n";

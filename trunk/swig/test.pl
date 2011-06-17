#!/usr/bin/perl

print "Started\n";
use exo;

#currently exodus function arguments must given as exodus variables
#and cannot be given as normal quoted string constants.
#that will be fixed soon.
#apologies perl monks, my 1st perl prog

$exo=new exo::var("");

if (!$exo->connect($exo)) {
	print "no default database connection\n";
} else {

	$filename=new exo::var("tempdbfile564");
	$options=new exo::var("");

	if ($exo->createfile($filename,$options)) {
		print "$filename file created\n";
	}else{
		print "$filename file not created. maybe already exists. try deletefile $filename\n";
	}

	#write 10 records
	for ($ii=1;$ii<=100;++$ii) {
		$record=$exo->date();
		$id=new exo::var($ii);
		if (!$record->write($filename,$id)) {
			last;
		}
		print $id," ";
	}
	print "\n";

	if ($filename->deletefile()) {
		print "$filename file deleted\n";
	}
}

$D=new exo::var("D");
$MT=new exo::var("MT");
print $D,"\n";
print $MT,"\n";

print "Pick date: ", $exo->date(), " is ", $exo->date()->oconv($D), "\n";
print "Pick time: ", $exo->time(), " is ", $exo->time()->oconv($MT),"\n";

print "Finished\n"

#!/usr/bin/perl

$status = 0;

if (@ARGV == 0) {
	$ARGV[0] = "-h";
}

for ($i=0; $i<@ARGV; $i++) {
	if ($ARGV[$i] eq "--shift") {
		$opt{'shift'} = $ARGV[$i+1];
		$i++;
	}
	elsif ($ARGV[$i] eq "--shifteach") {
		$opt{'shifteach'} = $ARGV[$i+1];
		$i++;
	}
	elsif ($ARGV[$i] eq "--verbose") {
		$opt{'verbose'} = 1;
	}
	elsif (($ARGV[$i] eq "--help") or ($ARGV[$i] eq "-h")) {
		print "\n";
		print "Usage: submerge.pl [OPTIONS] INPUT_FILES\n";
		print "\n";
		print "INPUT_FILES are one or more SRT files. \n";
		print "There are 2 ways to specify the duration for each file: \n";
		print "1. Specify explicitly the duration in seconds after each file, separating it from file name with a pipe symbol:\n";
		print "   abc.srt|12345\n";
		print "2. Subtitle files should all have an .srt extension.\n";
		print "   For each SRT file the corresponding video file should be in the same directory and with the same name \n";
		print "   except the extension, e.g. for abc.srt there should be abc.avi or abc.mpg, or abc.mkv etc.\n";
		print "\n";
		print "Output is written to STDOUT.\n";
		print "\n";
		print "Options:\n";
		print "--shift      Shift each subtitle with the specified number of milliseconds\n";
		print "--shifteach  Shift subtitles from each file with the specified number of milliseconds, multiplied by the file number\n";
		print "--verbose    Print additional info to STDERR\n";
		print "\n";
	}
	else {
		push @files, $ARGV[$i];
	}
}

$counter = 0;
$offset = 0;

for ($i=0; $i<@files; $i++) {
	$status = 0;
	$video_filename = "";

	print STDERR "Processing file: " . $files[$i] . "\n" if $opt{'verbose'};

	@dir_parts = split(/\//, $files[$i]);
	$dir = "";
	if (@dir_parts > 1) {
		if (substr($files[$i], 0, 1) eq "/") {
			$dir = "/";
		}
		for ($j=0; $j<(@dir_parts - 1); $j++) {
			$dir .= "/" . $dir_parts[$j];
		}
		$filename = $dir_parts[@dir_parts - 1];
	}
	else {
		$dir = ".";
		$filename = $files[$i];
	}
	print STDERR "Dir is " . $dir . "\n" if $opt{'verbose'};

	if ($filename =~ /|/) {
		@filename_parts = split(/|/, $filename);
		$filename = $filename_parts[0];
		$ffmpeg_duration = $filename_parts[1];
		$duration = 1000 * $ffmpeg_duration;
	}
	else {
		opendir(DIR, $dir) || die "Can't open dir $dir";
		@filenames = readdir(DIR);
		$tmp_filename = $files[$i];
		$tmp_filename =~ s/\.srt$//g;
		for ($j=0; $j<@filenames; $j++) {
			if ($filenames[$j] !~ /\.srt$/) {
				if ($filenames[$j] =~ /$tmp_filename/) {
					$video_filename = $filenames[$j];
					print STDERR "Found video file " . $video_filename . "\n" if $opt{'verbose'};
				}
			}
		}

		if ($video_filename eq "") {die "Cannot find matching video for subtitles file $files[$i]";}
	
		$video_filename = $dir . "/" . $video_filename;

		print STDERR "Checking duration of video file: " . $video_filename . "\n" if $opt{'verbose'};

		$ffmpeg_duration = `ffmpeg -i \"$video_filename\" 2>&1 | grep \"Duration\" | cut -d \' \' -f 4 | sed s/,//`;
		chomp $ffmpeg_duration;
		$duration = &ffmpeg2time($ffmpeg_duration);
	}

	print STDERR "Duration of video file: " . $ffmpeg_duration . " (" . $duration . " ms)\n" if $opt{'verbose'};

	open (FILE, $files[$i]) || die "Cannot open file $files[$i]!";
	while (!eof(FILE)) {
		$line = <FILE>;
		chomp $line;

		if (($status == 0) and ($line =~ /^\d+$/)) {
			$counter ++;
			print $counter . "\n";
			$status = 1;
			next;
		}
	
		elsif (($status == 1) and ($line =~ /-->/)) {
			@parts = split(/\ /, $line);
			$from = $parts[0];
			$to = $parts[2];

			$tmp_from = &time2sec($from);
			$tmp_from += $offset;

			$tmp_to = &time2sec($to);
			$tmp_to += $offset;

			if ($opt{'shift'} != 0) {
				$tmp_from += $opt{'shift'};
				$tmp_to += $opt{'shift'};
			}

			if ($opt{'shifteach'} != 0) {
                                $tmp_from += (($i + 1) * $opt{'shifteach'});
                                $tmp_to += (($i + 1) * $opt{'shifteach'});
			}

			$from = &sec2time($tmp_from);
			$to = &sec2time($tmp_to);

			print $from . " --> " . $to . "\n";
			$status = 2;
			next;
		}

		elsif ($status == 2) {
			print $line . "\n";
			$line =~ s /\s//g;
			if ($line =~ /^$/) {
				$status = 0;
			}
		}
	}
	$offset += $duration;
}

sub time2sec {
#00:04:52,488
	(my $hrs, my $min, my $sec)  = split(/:/, $_[0]);
	(my $sec, $msec) = split(/,/, $sec);
	my $time = $msec + 1000 * $sec + 60000 * $min + 3600000 * $hrs;
	return $time;
}


sub sec2time {
	my $hrs = int ($_[0] / 3600000);
	if (length($hrs) == 1) {$hrs = "0" .  $hrs;}
	my $rem = $_[0] % 3600000;

	my $min = int ($rem / 60000) ;
	if (length($min) == 1) {$min = "0" .  $min;}
	$rem = $rem % 60000;

	my $sec = int ($rem / 1000);
	if (length($sec) == 1) {$sec = "0" .  $sec;}
	my $msec = $rem % 1000;

	if (length($msec) == 1) {$msec = "00" .  $msec;}
	elsif (length($msec) == 2) {$msec = "0" .  $msec;}

	my $time = $hrs . ":" . $min . ":" . $sec . "," . $msec;
	return $time;
}

sub ffmpeg2time {
#ffmpeg -i Les\ Mis/Video/reel01.mpg 2>&1 | grep "Duration" | cut -d ' ' -f 4 | sed s/,//
#00:09:18.07
	(my $hrs, my $min, my $sec)  = split(/:/, $_[0]);
	(my $sec, $msec) = split(/\./, $sec);
	my $time = 10 * $msec + 1000 * $sec + 60000 * $min + 3600000 * $hrs;
	return $time;
}

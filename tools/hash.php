<?php
function calc_hash_tbl($u, $n = 256, $m = 16) {
	    $h = sprintf("%u", crc32($u));
		    $h1 = intval($h / $n);
		    $h2 = $h1 % $n; 
			    $h3 = base_convert($h2, 10, $m);
			    return $h3;
}

echo calc_hash_tbl("d32abcd", 8, 10) . "\n";

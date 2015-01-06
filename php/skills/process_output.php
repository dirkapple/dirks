<?php
for ($i = 0; $i <= 50; $i++) {
	  printf("progress: [%-50s] %d%%\r", str_repeat('#',$i), $i * 2);
	  usleep(1000 * 100);
}
echo "\n";
echo "Done.\n";

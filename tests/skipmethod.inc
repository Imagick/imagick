<?php

foreach ($requiredMethods as $requiredMethod) {
	if (method_exists('Imagick', $requiredMethod) == false) {
		die("skip Imagick not compiled with $requiredMethod method");
	}
}

?>

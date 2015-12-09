<?php


/**
 * This is a simple script to analyze symbols defined or needed by Imagick.
 * It is an attempt to check verify that the version info that Imagick is assuming
 * is true about ImageMagick is actually true.
 */

class SymbolChecker
{
	private $requiredSymbols = [];
	private $providedSymbols = [];
	private $exclusionPatterns = [];
	private $exclusions = [];

	public function addExclusionRegex($pattern)
	{
		$this->exclusionPatterns[] = $pattern;
	}

	public function addExclusion($string)
	{
		$this->exclusions[] = $string;
	}

	public function readSymbolsFromFile($imagickSymbols, $isImagick)
	{
		$lines = file($imagickSymbols);

		if ($lines === false) {
			echo "Failed to read $imagickSymbols \n";
			exit(-1);
		}

		foreach ($lines as $line) {
			$line = trim($line);
			$matched = preg_match('#(.*)(A|B|D|R|T|U|w) (.*)#', $line, $matches);
			if (!$matched) {
				echo "Failed to match line: ".$line."\n";
				continue;
			}
		
			$char = $matches[2];
			$symbol = $matches[3];
		
			switch($char) {
				case('U'): {
					if ($isImagick) {
						$this->requiredSymbols[] = trim($symbol);
					}
					break;
				}
		
				case('T'): {
					$this->providedSymbols[] = trim($symbol);
					break;
				}
			}
		}
	}
	
	
	function getMissingSymbols()
	{
		$missing = [];

		foreach ($this->requiredSymbols as $required) {
			if (in_array($required, $this->providedSymbols) == true) {
				continue;
			}

			foreach ($this->exclusionPatterns as $exclusionPattern) {
				if (preg_match("#".$exclusionPattern."#", $required)) {
					continue 2;
				}
			}

			if (in_array($required, $this->exclusions)) {
				continue;
			}

			$missing[] = $required;
		}

		return $missing;
	}
}


$symbolChecker = new SymbolChecker();

$symbolChecker->readSymbolsFromFile(__DIR__."/symbols.imagick.txt", true);
$symbolChecker->readSymbolsFromFile(__DIR__."/symbols.magickcore.txt", false);
$symbolChecker->readSymbolsFromFile(__DIR__."/symbols.magickwand.txt", false);

$symbolChecker->addExclusionRegex("^zend_.*");
$symbolChecker->addExclusionRegex("^_.*");
$symbolChecker->addExclusionRegex("^php_.*");
$symbolChecker->addExclusionRegex("^str.*");
$symbolChecker->addExclusionRegex("^add_.*");

$symbolChecker->addExclusionRegex("^access.*");
$symbolChecker->addExclusionRegex("^convert_to_.*");
$symbolChecker->addExclusionRegex("^fclose.*"); 
$symbolChecker->addExclusionRegex("^fopen.*"); 
$symbolChecker->addExclusionRegex("^fprintf.*"); 
$symbolChecker->addExclusionRegex("^instanceof_function_ex.*"); 
$symbolChecker->addExclusionRegex("^memcpy.*"); 
$symbolChecker->addExclusionRegex("^object_properties_init.*"); 
$symbolChecker->addExclusionRegex("^setlocale.*");

$symbolChecker->addExclusionRegex("^atoi.*");
$symbolChecker->addExclusionRegex("^free.*");
$symbolChecker->addExclusionRegex("^sqrt.*");

$symbolChecker->addExclusion("core_globals_id");
$symbolChecker->addExclusion("executor_globals_id");
$symbolChecker->addExclusion("ts_allocate_id");
$symbolChecker->addExclusion("tsrm_get_ls_cache");
$symbolChecker->addExclusion("virtual_access");
$symbolChecker->addExclusion("OnUpdateBool");
$symbolChecker->addExclusion("ap_php_snprintf"); 
$symbolChecker->addExclusion("core_globals"); 
$symbolChecker->addExclusion("display_ini_entries"); 
$symbolChecker->addExclusion("empty_fcall_info_cache"); 
$symbolChecker->addExclusion("executor_globals"); 
$symbolChecker->addExclusion("expand_filepath"); 
$symbolChecker->addExclusion("spl_ce_Countable"); 
$symbolChecker->addExclusion("spprintf");
$symbolChecker->addExclusion("ts_resource_ex");



$missingList = $symbolChecker->getMissingSymbols();

if (count($missingList) == 0) {
	exit(0);
}

echo "Detected missing symbols in imagick library:";
foreach ($missingList as $missing) {
	echo "$missing\n";
}

exit(-1);
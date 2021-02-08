--TEST--
Check if study is loaded
--SKIPIF--
<?php
if (!extension_loaded('study')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "study" is available';
?>
--EXPECT--
The extension "study" is available

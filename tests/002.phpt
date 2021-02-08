--TEST--
study_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('study')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = study_test1();

var_dump($ret);
?>
--EXPECT--
The extension study is loaded and working!
NULL

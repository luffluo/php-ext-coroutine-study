<?php

$a = 'a';
$b = 'b';

\Study\Coroutine::create(function ($a, $b = null) {
    echo $a . "\n";
    echo $b . "\n";
    echo "success\n";
}, $a, $b);

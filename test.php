<?php

$a = \Study\Coroutine::create(function () {
    return "Hi, i'm a callable \n";
});

var_dump($a);

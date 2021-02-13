<?php

use Study\Coroutine;

function deferFunc1()
{
    echo "in defer " . __FUNCTION__ . PHP_EOL;
}

function deferFunc2()
{
    echo "in defer " . __FUNCTION__ . PHP_EOL;
}

function deferFunc3()
{
    echo "in defer " . __FUNCTION__ . PHP_EOL;
}

function task()
{
    echo "task coroutine start" . PHP_EOL;
    Coroutine::defer('deferFunc1');
    Coroutine::defer('deferFunc2');
    Coroutine::defer('deferFunc3');
    echo "task coroutine end" . PHP_EOL;
}

$cid1 = Coroutine::create('task');

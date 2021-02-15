<?php

$server = new \Study\Coroutine\Server('127.0.0.1', 8080);
var_dump($server);

die();

go(function () {
    echo "before sleep" . PHP_EOL;
    Co::sleep(10);
    echo "after sleep" . PHP_EOL;
});

go(function () {
    echo "before sleep" . PHP_EOL;
    Co::sleep(20);
    echo "after sleep" . PHP_EOL;
});

echo "main co" . PHP_EOL;

Co::scheduler();
die();

Co::sleep(1);
Co::sleep(0.1);
Co::sleep(0.01);
Co::sleep(0.001);
Co::sleep(0.0001);
die();

study_timer_test();
die();

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
    Co::defer('deferFunc1');
    Co::defer('deferFunc2');
    Co::defer('deferFunc3');
    echo "task coroutine end" . PHP_EOL;
}

$cid1 = go('task');

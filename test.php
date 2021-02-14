<?php

$cid = go(function () {
    echo "before sleep" . PHP_EOL;
    Co::sleep(1);
    echo "after sleep" . PHP_EOL;
});
echo "main co" . PHP_EOL;
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

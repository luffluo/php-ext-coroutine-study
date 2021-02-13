<?php

use Study\Coroutine;

function task($arg)
{
    $cid = Coroutine::getCid();
    echo "coroutine [{$cid}] create successfully" . PHP_EOL;
    Coroutine::yield();
    echo "coroutine [{$cid}] be resumed" . PHP_EOL;
}

echo "main coroutine" . PHP_EOL;
$cid1 = Coroutine::create('task', 'a');
echo "main coroutine" . PHP_EOL;
$cid2 = Coroutine::create('task', 'b');

if (Coroutine::isExist($cid1)) {
    echo "coroutine [{$cid1}] is existent\n";
} else {
    echo "coroutine [{$cid1}] is non-existent\n";
}

if (Coroutine::isExist($cid2)) {
    echo "coroutine [{$cid2}] is existent\n";
} else {
    echo "coroutine [{$cid2}] is non-existent\n";
}

echo "main coroutine" . PHP_EOL;
Coroutine::resume($cid1);
echo "main coroutine" . PHP_EOL;
Coroutine::resume($cid2);

if (Coroutine::isExist($cid1)) {
    echo "coroutine [{$cid1}] is existent\n";
} else {
    echo "coroutine [{$cid1}] is non-existent\n";
}

if (Coroutine::isExist($cid2)) {
    echo "coroutine [{$cid2}] is existent\n";
} else {
    echo "coroutine [{$cid2}] is non-existent\n";
}

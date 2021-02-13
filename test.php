<?php

function task($arg)
{
    $cid = \Study\Coroutine::getCid();
    echo "coroutine [{$cid}] create successfully" . PHP_EOL;
    \Study\Coroutine::yield();
    echo "coroutine [{$cid}] be resumed" . PHP_EOL;
}

echo "main coroutine" . PHP_EOL;
$cid1 = \Study\Coroutine::create('task', 'a');
echo "main coroutine" . PHP_EOL;
$cid2 = \Study\Coroutine::create('task', 'b');


echo "main coroutine" . PHP_EOL;
\Study\Coroutine::resume($cid1);
echo "main coroutine" . PHP_EOL;
\Study\Coroutine::resume($cid2);
echo "main coroutine" . PHP_EOL;
\Study\Coroutine::resume($cid2);

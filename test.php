<?php

function task($n, $arg)
{
    echo "coroutine [{$n}] create successfully" . PHP_EOL;
    \Study\Coroutine::yield();
    echo "coroutine [{$n}] be resumed" . PHP_EOL;
}

echo "main coroutine" . PHP_EOL;
\Study\Coroutine::create('task', 1, 'a', 'b');
echo "main coroutine" . PHP_EOL;
\Study\Coroutine::create('task', 2, 'c', 'd');


echo "main coroutine" . PHP_EOL;
\Study\Coroutine::resume(1);
echo "main coroutine" . PHP_EOL;
\Study\Coroutine::resume(2);

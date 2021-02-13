<?php

function task($n, $arg)
{
    echo "coroutine [{$n}] create successfully" . PHP_EOL;
    if (2 === $n) {
        \Study\Coroutine::yield();
    }

    echo 'Hi im 2' . PHP_EOL;
}

echo "main coroutine" . PHP_EOL;
\Study\Coroutine::create('task', 1, 'a', 'b');
echo "main coroutine" . PHP_EOL;
\Study\Coroutine::create('task', 2, 'c', 'd');

<?php

function task($n, $a, $b)
{
    if (1 === $n) {
        echo "coroutine [{$n}] create successfully" . PHP_EOL;
    }

    if (2 === $n) {
        echo "coroutine [{$n}] create successfully" . PHP_EOL;
    }

    echo $a . PHP_EOL;
    echo $b . PHP_EOL;
}

echo "main coroutine" . PHP_EOL;
\Study\Coroutine::create('task', 1, 'a', 'b');
echo "main coroutine" . PHP_EOL;
\Study\Coroutine::create('task', 2, 'c', 'd');

<?php

go(function() {
    $server = new \Study\Coroutine\Server('127.0.0.1', 8080);

    while (1) {
        $connfd = $server->accept();

        while (1) {
            $msg = $server->recv($connfd);
            var_dump($msg);
        }
    }
});

Co::scheduler();
exit;

while (1) {
    $connfd = $server->accept();

    while (1) {
        $buf = $server->recv($connfd);
        if (false === $buf) {
            break;
        }

        $server->send($connfd, "Hello");
    }
}
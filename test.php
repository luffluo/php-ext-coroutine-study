<?php

go(function() {
    var_dump('server start');
    $server = new \Study\Coroutine\Server('127.0.0.1', 8080);
    var_dump('server created');

    while (1) {
        var_dump('accept start');
        $connfd = $server->accept();
        var_dump('accept connectied ' . $connfd);

        go(function() use ($server, $connfd) {
            while (1) {
                var_dump('recv start');
                $msg = $server->recv($connfd);
                $server->send($connfd, $msg);
                var_dump('send end');
            }
        });
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

<?php

study_event_init();

go(function() {
    $server = new \Study\Coroutine\Server('127.0.0.1', 8080);

    while (true) {
        $connfd = $server->accept();

        go(function($server, $connfd) {
            $buf = $server->recv($connfd);
            $responseStr = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: 11\r\n\r\nhello world\r\n";
            $server->send($connfd, $responseStr);
            $server->close($connfd);
            Co::sleep(0.01);
        }, $server, $connfd);
    }
});


study_event_wait();
/**
 * 管理者画面ログ表示 socket通信
 *
 */
!function(){
    let ws = new WebSocket('ws://localhost:8085/admin');

    ws.onopen = function(){
        console.log('Websocket connectin start');
    }

    ws.onerror = function(error) {
        console.error('Websocket error: ' + error);
    }

}();
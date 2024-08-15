document.getElementById('connectionForm').addEventListener('submit', function(event) {
    event.preventDefault();

    const ipAddress = document.getElementById('ipAddress').value;
    const port = document.getElementById('port').value;
    const responseDiv = document.getElementById('response');

    fetch('/submit', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ ipAddress: ipAddress, port: port })
    })
        .then(response => response.json())
        .then(data => {
            responseDiv.textContent = 'Response from server: ' + JSON.stringify(data);
        })
        .catch(error => {
            responseDiv.textContent = 'Error: ' + error;
        });
});

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



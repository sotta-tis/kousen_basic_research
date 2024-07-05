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

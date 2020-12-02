const char mainPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang='ko'>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Finder IOT Edge System - Sensor</title>
  <style>
    body {
      text-align: center;
    }
  </style>
</head>

<body>
  <a href='/save'>WIFI 설정</a>
  <a href='/setIp'>IP 설정</a>
  <div id='root'></div>

  <script>
    const getData = () => {
      fetch('/data')
        .then(response => response.json())
        .then(display)
        .catch(console.log);
    };

    const display = data => {
      let html = '';

      data.map((value, index) => {
        html += `<p>${value.sensor}: ${value.data}</p>`;
      });

      document.querySelector('#root').innerHTML = html;
    };

    window.addEventListener('beforeunload', () => {
      clearInterval(scanInterval);
    });

    getData();
    const scanInterval = setInterval(() => getData(), 2000);
  </script>
</body>
</html>
)=====";

void handleMainPage() {
  server.send(200, "text/html", mainPage);
}

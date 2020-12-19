const char setIpPage[] PROGMEM = R"=====(
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
    p {
      margin: 5px;
    }
  </style>
</head>

<body>
  <form action='/save' method='POST' onsubmit='onClickSetIp(event)'>
    <p><input type='text' id='inputIp' name='ip' placeholder='IP'></p>
    <p><input type='text' id='inputGateway' name='gateway' placeholder='Gateway'></p>
    <p><input type='text' id='inputNetMsk' name='netMsk' placeholder='Subnet Mask'></p>
    <input type='submit' value='저장'>
  </form>

  <script>
    const onClickSetIp = e => {
      e.preventDefault();

      const ip = JSON.stringify(
        document.querySelector('#inputIp').value
          .split('.')
          .map((value, index) => value * 1)
      );
      const gateway = JSON.stringify(
        document.querySelector('#inputGateway').value
          .split('.')
          .map((value, index) => value * 1)
      );
      const netMsk = JSON.stringify(
        document.querySelector('#inputNetMsk').value
          .split('.')
          .map((value, index) => value * 1)
      );

      const formData = new FormData();
      formData.append('ip', ip);
      formData.append('gateway', gateway);
      formData.append('netMsk', netMsk);

      fetch('/setIp', { method: 'post', body: formData })
        .then(response => response.text())
        .then(getSetIpResult)
        .catch(console.log);
    };

    const getSetIpResult = data => {
      if (data === 'success')
        alert('입력값이 저장되었습니다');
      else
        alert('입력값 저장에 실패하였습니다');
    };
  </script>
</body>
</html>
)=====";

void handleSetIpPage() {
  server.send(200, "text/html", setIpPage);
}

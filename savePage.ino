const char savePage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang='ko'>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Finder IOT Edge System - Sensor</title>
  <style>
    body { text-align: center; }
    table { margin: auto; text-align: left; }
    p { margin: 5px; }
    .tdNumber { text-align: right; }
  </style>
</head>

<body>
  <table>
    <thead id='scanThead'></thead>
    <tbody id='scanTbody'></tbody>
  </table>

  <form action='/save' method='POST' onsubmit='onClickSave(event)'>
    <p><input type='text' id='inputSsid' name='ssid' placeholder='SSID'></p>
    <p><input type='password' id='inputPass' name='pass' placeholder='Password'></p>
    <input type='submit' value='저장'>
  </form>

  <script>
    const getScanData = () => {
      fetch('/scan')
        .then(response => response.json())
        .then(display)
        .catch(console.log);
    };

    const display = data => {
      let html = '';

      const result = data.sort((next, prev) => next.signal <= prev.signal ? 1 : -1);
      result.map((value, index) => {
        html += `
              <tr>
                <td><a href='#' onclick='onClickSsid(event)'>${value.ssid}</a></td>
                <td>${value.lock ? 'lock' : 'unlock'}</td>
                <td class='tdNumber'>${value.signal}%</td>
              </tr>
            `;
      });
      document.querySelector('#scanThead').innerHTML = '<tr><th>검색된 WIFI</th></tr>';
      document.querySelector('#scanTbody').innerHTML = html;
    };

    const getSaveResult = data => {
      if (data === 'success')
        alert('입력값이 저장되었습니다\n무선 연결을 시도합니다');
      else
        alert('입력값 저장에 실패하였습니다');
    };

    const onClickSsid = e => {
      e.preventDefault();
      document.querySelector('#inputSsid').value = e.target.text;
    }

    const onClickSave = e => {
      e.preventDefault();
      const ssid = document.querySelector('#inputSsid').value;
      const pass = document.querySelector('#inputPass').value;

      if (ssid === '' || pass === '') return alert('WIFI 정보를 전부 입력해주세요');

      const formData = new FormData();
      formData.append('ssid', ssid);
      formData.append('pass', pass);

      fetch('/save', { method: 'post', body: formData })
        .then(response => response.text())
        .then(getSaveResult)
        .catch(alert);
    };

    window.addEventListener('beforeunload', () => {
      clearInterval(scanInterval);
    });

    getScanData();
    const scanInterval = setInterval(() => getScanData(), 5000);
  </script>
</body>
</html>
)=====";

void handleSavePage() {
  server.send(200, "text/html", savePage);
}

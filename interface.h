const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Smart Window Dashboard</title>
<style>
  @import url('https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;500;700&display=swap');
  body { 
    font-family: 'Roboto', sans-serif; 
    background-color: #f8f9fa; 
    color: #202124; 
    margin: 0; 
    padding: 20px; 
    display: flex; 
    flex-direction: column; 
    align-items: center; 
  }
  .container { 
    max-width: 600px; 
    width: 100%; 
    display: flex; 
    flex-direction: column; 
    gap: 20px; 
  }
  .card { 
    background: #fff; 
    border-radius: 24px; 
    padding: 24px; 
    box-shadow: 0 1px 3px rgba(0,0,0,0.12), 0 1px 2px rgba(0,0,0,0.24); 
  }
  .location { 
    font-size: 20px; 
    font-weight: 500; 
    margin-bottom: 24px; 
    color: #202124; 
  }
  .weather-main { 
    display: flex; 
    align-items: center; 
    justify-content: space-between; 
  }
  .temp-block { 
    display: flex; 
    align-items: flex-start; 
  }
  .w-icon { 
    width: 64px; 
    height: 64px; 
    margin-right: 12px; 
    display: none; 
  }
  .temp { 
    font-size: 72px; 
    line-height: 1; 
    font-weight: 400; 
  }
  .unit { 
    font-size: 24px; 
    margin-top: 8px; 
    color: #5f6368; 
  }
  .weather-details { 
    font-size: 15px; 
    color: #5f6368; 
    line-height: 1.6; 
    text-align: right; 
  }
  .weather-params {
    display: flex;
    gap: 16px;
    margin-top: 12px;
    border-bottom: 2px solid #fbd043;
    padding-bottom: 8px;
    color: #202124;
    font-weight: 500;
  }
  .forecast-container {
    display: flex;
    justify-content: space-between;
    margin-top: 24px;
    overflow-x: auto;
  }
  .forecast-item {
    display: flex;
    flex-direction: column;
    align-items: center;
    min-width: 50px;
  }
  .forecast-day {
    font-size: 14px;
    color: #202124;
    margin-bottom: 8px;
  }
  .forecast-icon {
    width: 40px;
    height: 40px;
    margin-bottom: 8px;
  }
  .forecast-temps {
    font-size: 14px;
    color: #202124;
    font-weight: 500;
  }
  .forecast-min {
    color: #70757a;
    font-weight: 400;
    margin-left: 6px;
  }
  .status-title {
    font-size: 16px;
    color: #5f6368;
    margin-bottom: 8px;
    text-align: center;
  }
  .status-aberta { 
    color: #1e8e3e; 
    font-size: 36px; 
    font-weight: 700; 
    text-align: center; 
  }
  .status-fechada { 
    color: #d93025; 
    font-size: 36px; 
    font-weight: 700; 
    text-align: center; 
  }
  .history-title {
    font-size: 18px;
    font-weight: 500;
    margin-bottom: 16px;
    color: #202124;
  }
  .history-list { 
    list-style: none; 
    padding: 0; 
    margin: 0; 
    font-size: 14px; 
  }
  .history-item { 
    padding: 12px 0; 
    border-bottom: 1px solid #f1f3f4; 
    display: flex; 
    justify-content: space-between; 
    color: #3c4043;
  }
  .history-item:last-child { 
    border-bottom: none; 
  }
  .history-date {
    color: #80868b;
    font-size: 13px;
  }
</style>
</head>
<body>
<div class="container">

  <div class="card">
    <div class="location">Curitiba - PR</div>
    <div class="weather-main">
      <div class="temp-block">
        <img id="w-icon" class="w-icon" src="" alt="Clima">
        <div class="temp" id="w-temp">--</div>
        <div class="unit">°C</div>
      </div>
      <div class="weather-details">
        <div id="w-desc" style="font-weight: 500; color: #202124; font-size: 18px; margin-bottom: 4px;">Atualizando...</div>
        <div id="w-hum">Umidade: --%</div>
        <div id="w-wind">Vento: -- km/h</div>
      </div>
    </div>
    <div class="weather-params">
      <span>Temperatura</span>
      <span style="color: #70757a; font-weight: 400;">Chuva</span>
      <span style="color: #70757a; font-weight: 400;">Vento</span>
    </div>
    <div class="forecast-container" id="forecast-list">
       <span style="font-size:14px; color:#5f6368;">Buscando previsão...</span>
    </div>
  </div>

  <div class="card">
    <div class="status-title">Status Físico da Janela</div>
    <div id="window-status" class="status-aberta">CARREGANDO...</div>
  </div>

  <div class="card">
    <div class="history-title">Últimos Eventos</div>
    <ul class="history-list" id="history-list">
      <li class="history-item">Sincronizando com o motor...</li>
    </ul>
  </div>

</div>

<script>
  const apiKey = "COLOQUE A CHAVE DA SUA API DA OPENWEATHER AQUI";
  const city = "Curitiba,BR";

  async function getWeatherData() {
    // Bloco 1: Clima Atual
    try {
      let resCurrent = await fetch(`https://api.openweathermap.org/data/2.5/weather?q=${city}&units=metric&lang=pt_br&appid=${apiKey}`);
      let dataCurrent = await resCurrent.json();
      
      if(dataCurrent.cod === 200) {
        document.getElementById('w-temp').innerText = Math.round(dataCurrent.main.temp);
        let desc = dataCurrent.weather[0].description;
        document.getElementById('w-desc').innerText = desc.charAt(0).toUpperCase() + desc.slice(1);
        document.getElementById('w-hum').innerText = `Umidade: ${dataCurrent.main.humidity}%`;
        document.getElementById('w-wind').innerText = `Vento: ${Math.round(dataCurrent.wind.speed * 3.6)} km/h`;
        
        let iconUrl = `https://openweathermap.org/img/wn/${dataCurrent.weather[0].icon}@2x.png`;
        let iconImg = document.getElementById('w-icon');
        iconImg.src = iconUrl;
        iconImg.style.display = 'block';
      } else {
        document.getElementById('w-desc').innerText = "Erro: " + dataCurrent.message;
      }
    } catch(e) {
      document.getElementById('w-desc').innerText = "Erro de conexão API";
    }

    // Bloco 2: Previsão (Isolado do clima atual)
    try {
      let resForecast = await fetch(`https://api.openweathermap.org/data/2.5/forecast?q=${city}&units=metric&lang=pt_br&appid=${apiKey}`);
      let dataForecast = await resForecast.json();
      
      if(String(dataForecast.cod) === "200") {
        let dailyData = {};
        
        dataForecast.list.forEach(item => {
          let dt = new Date(item.dt * 1000);
          let dateStr = dt.toISOString().split('T')[0];
          
          if(!dailyData[dateStr]) {
            let dayName = dt.toLocaleDateString('pt-BR', {weekday: 'short'}).replace('.', '');
            dailyData[dateStr] = {
              day: dayName,
              temps: [],
              icon: item.weather[0].icon.replace('n', 'd')
            };
          }
          dailyData[dateStr].temps.push(item.main.temp);
        });

        let forecastHTML = '';
        let count = 0;
        
        for(let date in dailyData) {
          if(count > 4) break;
          let maxT = Math.round(Math.max(...dailyData[date].temps));
          let minT = Math.round(Math.min(...dailyData[date].temps));
          let dName = dailyData[date].day;
          let iconFile = dailyData[date].icon;
          
          forecastHTML += `
            <div class="forecast-item">
              <div class="forecast-day">${dName}</div>
              <img class="forecast-icon" src="https://openweathermap.org/img/wn/${iconFile}.png">
              <div class="forecast-temps">${maxT}°<span class="forecast-min">${minT}°</span></div>
            </div>
          `;
          count++;
        }
        
        document.getElementById('forecast-list').innerHTML = forecastHTML;
      } else {
         document.getElementById('forecast-list').innerHTML = `<span style="font-size:14px; color:#d93025;">Previsão indisponível: ${dataForecast.message}</span>`;
      }
    } catch(e) {
       document.getElementById('forecast-list').innerHTML = `<span style="font-size:14px; color:#d93025;">Falha ao carregar previsão.</span>`;
    }
  }

  async function updateESP32Data() {
    try {
      let resStatus = await fetch('/get_status');
      let statusTxt = await resStatus.text();
      let statusEl = document.getElementById('window-status');
      statusEl.innerText = statusTxt;
      
      if(statusTxt === "ABERTA") {
        statusEl.className = "status-aberta";
      } else {
        statusEl.className = "status-fechada";
      }

      let resHist = await fetch('/get_history');
      let histData = await resHist.json();
      let listEl = document.getElementById('history-list');
      listEl.innerHTML = "";
      
      histData.forEach(item => {
        let li = document.createElement('li');
        li.className = "history-item";
        li.innerHTML = `<span>${item.acao}</span><span class="history-date">${item.data}</span>`;
        listEl.appendChild(li);
      });
    } catch(e) {}
  }

  getWeatherData();
  updateESP32Data();
  
  setInterval(updateESP32Data, 2000);
  setInterval(getWeatherData, 600000);
</script>
</body>
</html>
)rawliteral";

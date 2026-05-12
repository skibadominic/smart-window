const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="utf-8">
    <title>Monitoramento Smart Window</title>
    <style>
        body { font-family: sans-serif; text-align: center; background-color: #f4f7f6; padding-top: 30px; }
        .card { background: white; max-width: 450px; margin: 0 auto; padding: 20px; border-radius: 15px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); }
        .status-box { font-size: 22px; margin: 20px 0; padding: 15px; border-radius: 10px; font-weight: bold; border: 2px solid #ccc; }
        .aberta { background-color: #d4edda; color: #155724; border-color: #c3e6cb; }
        .fechada { background-color: #f8d7da; color: #721c24; border-color: #f5c6cb; }
        table { width: 100%; border-collapse: collapse; margin-top: 20px; font-size: 14px; background: #fff; }
        th, td { padding: 12px; border-bottom: 1px solid #eee; text-align: left; }
        th { background-color: #f8f9fa; color: #666; text-transform: uppercase; font-size: 12px; }
        tr:last-child td { border-bottom: none; }
    </style>
</head>
<body>
    <div class="card">
        <h2>Smart Window</h2>
        <div id="statusDiv" class="status-box">Sincronizando...</div>
        
        <h3>Log de Atividades</h3>
        <table>
            <thead>
                <tr>
                    <th>Data e Hora</th>
                    <th>Evento</th>
                </tr>
            </thead>
            <tbody id="tabelaHistorico">
            </tbody>
        </table>
    </div>
    <script>
        function atualizar() {
            fetch('/get_status?' + new Date().getTime()).then(r => r.text()).then(t => {
                const d = document.getElementById('statusDiv');
                d.innerHTML = "Janela " + t;
                d.className = "status-box " + (t == "ABERTA" ? "aberta" : "fechada");
            });
            fetch('/get_history?' + new Date().getTime()).then(r => r.json()).then(dados => {
                const tbody = document.getElementById('tabelaHistorico');
                tbody.innerHTML = '';
                dados.forEach(evento => {
                    let tr = document.createElement('tr');
                    let tdData = document.createElement('td');
                    let tdAcao = document.createElement('td');
                    tdData.innerText = evento.data;
                    tdAcao.innerText = evento.acao;
                    tr.appendChild(tdData);
                    tr.appendChild(tdAcao);
                    tbody.appendChild(tr);
                });
            });
        }
        setInterval(atualizar, 3000);
        window.onload = atualizar;
    </script>
</body>
</html>
)=====";

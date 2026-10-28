#pragma once
// Interface web sincronizada com web/index.html. Nao edite os arquivos isoladamente.
const char PAGINA_WEB[] PROGMEM = R"CARRINHOHTML(
<!doctype html>
<html lang="pt-BR">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#101725">
<meta name="description" content="Controle e demonstração do carrinho ESP32 via Wi-Fi">
<title>Carrinho IoT | Controle</title>
<style>
:root{font-family:system-ui,-apple-system,"Segoe UI",Arial,sans-serif;color:#e9f0fa;background:#101725;color-scheme:dark}
*{box-sizing:border-box}body{margin:0;min-height:100vh;background:#101725}
main{width:min(940px,100%);margin:auto;padding:28px 16px 46px}
header{display:flex;justify-content:space-between;align-items:center;gap:12px;flex-wrap:wrap}
h1{font-size:clamp(25px,5vw,35px);margin:0}h2{font-size:17px;margin:0 0 16px}
p{line-height:1.6}.subtitle,.hint{color:#a4b4c9;font-size:13px}
.pill{border:1px solid #496080;border-radius:30px;padding:9px 13px;font-size:12px;color:#cddbeb}
.notice{background:#1b2940;border:1px solid #3b5374;border-radius:10px;padding:13px 16px;margin:20px 0;color:#dbe7f6;font-size:13px}
.layout{display:grid;grid-template-columns:1fr 1fr;gap:16px}
.panel{background:#192538;border:1px solid #34445c;border-radius:16px;padding:22px;min-width:0}
.joystick{display:flex;flex-direction:column;align-items:center}.joystick h2{align-self:stretch}
.pad{width:226px;height:226px;position:relative;touch-action:none;cursor:grab;border:2px solid #52698b;background:radial-gradient(circle,#243956,#111d2f);border-radius:50%}
.pad:active{cursor:grabbing}.pad:after{content:"";position:absolute;inset:18px;border:1px solid #415775;border-radius:50%;pointer-events:none}
.knob{width:76px;height:76px;position:absolute;top:calc(50% - 38px);left:calc(50% - 38px);border-radius:50%;background:#438bdf;border:2px solid #a3c9fb;display:grid;place-items:center;font-size:26px;pointer-events:none;will-change:transform}
.joystate{margin:18px 0 5px;font-weight:750;color:#d9e9ff}.small{font-size:12px;color:#adc0d8}
.setting{margin-top:16px}.slider-head{display:flex;align-items:center;justify-content:space-between;gap:10px;font-weight:650;font-size:14px}
output{color:#a8d6ff;font-weight:800}input[type=range]{display:block;width:100%;accent-color:#75b5ff;margin:16px 0}
.ends{display:flex;justify-content:space-between;gap:10px;font-size:11px;color:#acbfd8}
.modes{display:flex;gap:7px;margin-top:10px}.mode{flex:1;background:#27354b;border:1px solid #475c79;border-radius:8px;color:#d7e5f6;padding:10px;cursor:pointer}.mode.active{background:#3766a8;color:white}
.advanced{border-top:1px solid #34445c;margin-top:20px;padding-top:16px}.advanced summary{cursor:pointer;font-size:14px;font-weight:700}
.setting-note{font-size:12px;color:#b4c5db}.meter{display:flex;flex-wrap:wrap;gap:16px;font-size:12px;color:#adc4e3}
.speed-steps{display:flex;gap:10px}.speed-steps button{flex:1;border:1px solid #50749d;border-radius:9px;padding:11px;background:#274469;color:white;font-weight:700;cursor:pointer}
.actions{display:flex;align-items:center;justify-content:space-between;gap:14px;flex-wrap:wrap;margin-top:17px}
.status{font-weight:700;color:#99e8be;font-size:13px}.status.error{color:#ffb4be}
.stop{background:#c83953;color:white;border:1px solid #f9778c;border-radius:10px;padding:15px 25px;font-weight:900;cursor:pointer}
button:focus-visible,summary:focus-visible,.pad:focus-visible,input:focus-visible{outline:3px solid #ffcf71;outline-offset:3px}
footer{font-size:12px;line-height:1.65;color:#aebed1;margin-top:22px}
@media(max-width:670px){.layout{grid-template-columns:1fr}.panel{padding:17px}.stop{width:100%}.pad{width:214px;height:214px}}
@media(prefers-reduced-motion:reduce){*{scroll-behavior:auto!important;transition:none!important}}
</style>
</head>
<body>
<main>
<header>
  <div><h1>Carrinho IoT</h1><p class="subtitle">Controle Wi-Fi por ESP32 · Projeto acadêmico</p></div>
  <span class="pill" id="modeLabel">Verificando...</span>
</header>
<div class="notice" id="notice" role="note">Carregando interface...</div>
<div class="layout">
  <section class="panel joystick" aria-labelledby="controle-title">
    <h2 id="controle-title">Direção</h2>
    <div class="pad" id="pad" tabindex="0" role="group" aria-label="Joystick: arraste para dirigir e solte para parar">
      <div class="knob" id="knob" aria-hidden="true">✥</div>
    </div>
    <div class="joystate" id="direction">Parado</div>
    <p class="small">Arraste o círculo azul. Solte para parar.</p>
  </section>
  <section class="panel" aria-labelledby="ajustes-title">
    <h2 id="ajustes-title">Velocidade</h2>
    <div class="slider-head"><label for="speed">Limite de velocidade</label><output id="speedValue" for="speed">30%</output></div>
    <input id="speed" type="range" min="30" max="100" step="10" value="30">
    <div class="speed-steps">
      <button type="button" data-speed-step="-10" aria-label="Desacelerar 10 por cento">− Desacelerar</button>
      <button type="button" data-speed-step="10" aria-label="Acelerar 10 por cento">+ Acelerar</button>
    </div>
    <details class="advanced">
      <summary>Faróis e sensor de luz</summary>
      <p class="hint">Faróis automáticos quando escurece. Ajustes adicionais são opcionais.</p>
      <div class="modes" aria-label="Modo do farol">
        <button type="button" class="mode active" data-farol="auto">Auto</button>
        <button type="button" class="mode" data-farol="ligar">Ligar</button>
        <button type="button" class="mode" data-farol="desligar">Desligar</button>
      </div>
      <p class="setting-note" id="farolInfo">O sensor decide quando acender.</p>
      <div class="setting">
        <div class="slider-head"><label for="sensitivity">Sensibilidade</label><output id="sensitivityValue" for="sensitivity">300</output></div>
        <input id="sensitivity" type="range" min="100" max="900" step="25" value="300">
        <div class="ends"><span>Menor sensibilidade</span><span>Maior sensibilidade</span></div>
        <p class="meter"><span>LDR: <strong id="ldr">--</strong></span><span>Farol: <strong id="farolStatus">--</strong></span></p>
      </div>
    </details>
  </section>
</div>
<div class="actions">
  <div class="status" id="status" role="status" aria-live="polite">Parado</div>
  <button type="button" class="stop" id="stop">■ PARAR</button>
</div>
<footer>Primeiro teste: rodas suspensas e velocidade baixa. Fora da rede do carrinho, este painel é apenas uma simulação; para controlar os motores, conecte-se ao Wi-Fi do ESP32 e abra <strong>http://192.168.4.1/</strong>. O botão PARAR é uma função de software, não um corte físico da energia.</footer>
</main>
<script>
(function(){
  "use strict";
  const $ = id => document.getElementById(id);
  const local = location.hostname === "192.168.4.1";
  const status = $("status"), pad = $("pad"), knob = $("knob");
  const tips = {
    auto:"O sensor de luz decide quando acender os LEDs.",
    ligar:"Os LEDs ficam ligados, mesmo com o ambiente claro.",
    desligar:"Faróis apagados manualmente; as setas continuam funcionando."
  };
  let activePointer = null, x = 0, y = 0, tick = null, lastSent = 0;
  let enviandoJoystick = false, joystickPendente = null;
  let farol = "auto", speed = 30, sensitivity = 300;
  $("modeLabel").textContent = local ? "● Rede local · ESP32" : "◉ Demonstração · Web";
  $("notice").textContent = local
    ? "Controle local: comandos enviados diretamente ao ESP32. O joystick para ao ser solto."
    : "Esta é uma demonstração interativa. Para mover o carrinho real, abra http://IP-DO-CARRINHO no navegador conectado ao mesmo Wi-Fi.";

  async function request(path, params, showError = true){
    if (!local) return null;
    try{
      const query = new URLSearchParams(params);
      const response = await fetch(path + "?" + query, {cache:"no-store"});
      if (!response.ok) throw new Error("HTTP " + response.status);
      status.classList.remove("error");
      return response;
    }catch(err){
      if (showError) { status.classList.add("error"); status.textContent = "Falha de comunicação: " + err.message; }
      return null;
    }
  }
  function direction(){
    if (!x && !y) return "Parado";
    if (y < -20) return x < -25 ? "Frente + esquerda" : x > 25 ? "Frente + direita" : "Frente";
    if (y > 20) return x < -25 ? "Ré + esquerda" : x > 25 ? "Ré + direita" : "Ré";
    return x < 0 ? "Esquerda" : "Direita";
  }
  function renderJoystick(){
    const range = (pad.clientWidth - knob.clientWidth) / 2;
    knob.style.transform = "translate(" + (x*range/100) + "px," + (y*range/100) + "px)";
    $("direction").textContent = direction();
    if (!local) status.textContent = "Simulação: " + direction();
    else if (!status.classList.contains("error")) status.textContent = direction();
  }
  // Envia uma requisicao por vez, evitando ultrapassagem do comando de parada.
  async function enviarJoystickEmOrdem(){
    if (!local || enviandoJoystick) return;
    enviandoJoystick = true;
    try {
      while (joystickPendente !== null) {
        const proximo = joystickPendente;
        joystickPendente = null;
        await request("/joystick", proximo, false);
      }
    } finally {
      enviandoJoystick = false;
    }
  }
  function transmit(force){
    const now = Date.now();
    if (!force && now - lastSent < 90) return;
    lastSent = now;
    joystickPendente = {x:x, y:y};
    enviarJoystickEmOrdem();
  }
  function pointerPosition(event){
    const rect = pad.getBoundingClientRect();
    const radius = (rect.width - knob.clientWidth)/2;
    let px = event.clientX - rect.left - rect.width/2;
    let py = event.clientY - rect.top - rect.height/2;
    const length = Math.hypot(px,py);
    if (length > radius){px = px * radius / length; py = py * radius / length;}
    x = Math.round(px / radius * 100); y = Math.round(py / radius * 100);
    if (Math.hypot(x,y) < 12){x=0;y=0;}
    renderJoystick();
    transmit(false);
  }
  function release(){
    if (tick !== null){clearInterval(tick);tick=null;}
    if (activePointer === null && x===0 && y===0) return;
    activePointer = null; x=0; y=0; renderJoystick();
    transmit(true);
  }
  pad.addEventListener("pointerdown",event=>{
    if(activePointer!==null) return;
    event.preventDefault();
    activePointer=event.pointerId;
    pad.setPointerCapture(event.pointerId);
    pointerPosition(event); transmit(true);
    tick=setInterval(()=>transmit(true),180);
  });
  pad.addEventListener("pointermove",event=>{if(activePointer===event.pointerId)pointerPosition(event);});
  ["pointerup","pointercancel","lostpointercapture"].forEach(ev=>pad.addEventListener(ev,release));
  $("stop").addEventListener("click",()=>{
    release();
    x=0; y=0; renderJoystick();
    transmit(true); // zero entra na mesma fila dos demais comandos
    status.textContent=local ? "Parada solicitada" : "Simulação: parado";
  });
  window.addEventListener("blur",release);
  document.addEventListener("visibilitychange",()=>{if(document.hidden)release();});

  function showFarol(value){
    farol=value;
    document.querySelectorAll("[data-farol]").forEach(b=>b.classList.toggle("active",b.dataset.farol===value));
    $("farolInfo").textContent=tips[value];
  }
  document.querySelectorAll("[data-farol]").forEach(button=>button.addEventListener("click",()=>{
    showFarol(button.dataset.farol);
    request("/config",{farol:farol});
  }));

  $("sensitivity").addEventListener("input",event=>{
    sensitivity=Number(event.target.value);
    $("sensitivityValue").textContent=sensitivity;
  });
  $("sensitivity").addEventListener("change",()=>request("/config",{limite:sensitivity}));

  $("speed").addEventListener("input",event=>{
    speed=Number(event.target.value);
    $("speedValue").textContent=speed+"%";
  });
  $("speed").addEventListener("change",()=>request("/config",{velocidade:speed}));
  document.querySelectorAll("[data-speed-step]").forEach(button=>{
    button.addEventListener("click",()=>{
      const slider=$("speed");
      speed=Math.max(30,Math.min(100,speed+Number(button.dataset.speedStep)));
      slider.value=String(speed);
      $("speedValue").textContent=speed+"%";
      request("/config",{velocidade:speed});
    });
  });

  async function refresh(){
    if(!local)return;
    const response=await request("/estado",{},false);
    if(!response)return;
    try{
      const data=await response.json();
      $("ldr").textContent=data.luz;
      $("farolStatus").textContent=data.farolLigado?"Ligado":"Desligado";
      if (activePointer===null) $("direction").textContent=data.movimento==="parar"?"Parado":data.movimento;
      if (data.modoFarol && data.modoFarol!==farol) showFarol(data.modoFarol);
      if (Number.isFinite(Number(data.velocidade)) && document.activeElement !== $("speed")) {
        speed = Number(data.velocidade);
        $("speed").value = speed;
        $("speedValue").textContent = speed + "%";
      }
      if (Number.isFinite(Number(data.limite)) && document.activeElement !== $("sensitivity")) {
        sensitivity = Number(data.limite);
        $("sensitivity").value = sensitivity;
        $("sensitivityValue").textContent = sensitivity;
      }
    }catch(_){}
  }
  renderJoystick();
  refresh();
  setInterval(refresh,2500);
})();
</script>
</body>
</html>
)CARRINHOHTML";

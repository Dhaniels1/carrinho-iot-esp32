#pragma once
// Mesma pagina de pagina.html, embutida no firmware da etapa 04.
const char PAGINA_WEB[] PROGMEM = R"ETAPA04HTML(
<!doctype html>
<html lang="pt-BR">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#080b10">
<meta name="description" content="Etapa 04: controle do carrinho ESP32 por navegador">
<title>Carrinho IoT | Etapa 04</title>
<style>
:root{font:16px system-ui,-apple-system,"Segoe UI",sans-serif;color:#f7f9ff;background:#080b10;color-scheme:dark}
*{box-sizing:border-box}
body{margin:0;min-height:100vh;background:radial-gradient(circle at 55% 0,#192b46 0,#0b101a 38%,#080b10 75%)}
main{width:min(1000px,100%);margin:auto;padding:25px 20px 44px}
header{display:flex;justify-content:space-between;align-items:center;gap:15px;flex-wrap:wrap;margin-bottom:22px}
.eyebrow{font-size:11px;font-weight:800;letter-spacing:.15em;color:#7caee9;margin:0 0 8px}
h1{font-size:clamp(25px,4vw,36px);margin:0;letter-spacing:-.04em}
.subtitle{font-size:13px;color:#96a8c2;margin:7px 0 0}
.pill{border:1px solid #2b3f60;background:#142139;border-radius:99px;padding:9px 13px;font-size:12px;color:#bdcce0}
.notice{background:#17253b;border:1px solid #2e415f;border-radius:12px;padding:13px 15px;margin:0 0 17px;color:#b8c9de;font-size:13px;line-height:1.55}
.top{display:grid;grid-template-columns:1fr 1.05fr;gap:18px;align-items:stretch}
.panel{background:#0e131c;border:1px solid #263141;border-radius:19px;padding:22px;box-shadow:0 16px 42px #0004}
h2{font-size:16px;margin:0 0 17px}
.joystick-panel{display:flex;flex-direction:column;align-items:center;justify-content:center;min-height:340px}
.joystick-panel h2{align-self:stretch}
.pad{width:234px;height:234px;position:relative;touch-action:none;cursor:grab;border:2px solid #39485d;background:radial-gradient(circle,#15273e 0,#090f18 68%);border-radius:50%;box-shadow:inset 0 0 28px #0008;outline-offset:4px}
.pad:active{cursor:grabbing}
.pad:before{content:"";position:absolute;inset:27px;border-radius:50%;border:1px solid #2a3a4d;pointer-events:none}
.pad:after{content:"";position:absolute;left:50%;top:50%;width:83%;height:1px;background:#243247;transform:translate(-50%,-50%);pointer-events:none}
.vertical{position:absolute;left:50%;top:9%;height:82%;width:1px;background:#243247;pointer-events:none}
.knob{position:absolute;left:calc(50% - 39px);top:calc(50% - 39px);width:78px;height:78px;border-radius:50%;border:1px solid #6ba8e6;background:linear-gradient(140deg,#5db5f5,#245eaf);box-shadow:0 7px 22px #0a4eab80;display:grid;place-items:center;font-size:26px;pointer-events:none;will-change:transform;z-index:1}
.joystate{font-size:13px;color:#c1d4ed;margin:16px 0 6px;min-height:20px}
.dot{width:7px;height:7px;background:#71cfa7;display:inline-block;border-radius:50%;margin-right:7px}
.small{font-size:12px;color:#91a3bc;text-align:center;line-height:1.6}
.settings{display:flex;flex-direction:column;justify-content:center;gap:19px}
.field-top{font-size:11px;font-weight:800;letter-spacing:.11em;color:#79b9ee;margin:0}
.feature{border:1px solid #2a3a50;background:#131d2b;padding:17px 18px;border-radius:13px}
.feature-head{display:flex;justify-content:space-between;gap:12px;align-items:center}
.feature-head strong{font-size:14px}
.feature-badge{background:#164c40;border:1px solid #2b7a63;border-radius:30px;color:#b0edd4;padding:5px 9px;font-size:11px}
.feature p{color:#a3b5ce;font-size:12px;line-height:1.7;margin:9px 0 0}
.step-list{display:grid;gap:12px}
.step{display:flex;gap:13px;align-items:center;color:#bdcce0;font-size:13px}
.step i{flex-shrink:0;width:27px;height:27px;border-radius:9px;display:grid;place-items:center;background:#223755;border:1px solid #3e5a80;font-style:normal;color:#9ecbff;font-weight:800;font-size:11px}
.bottom{margin-top:18px}
.slider-head{display:flex;justify-content:space-between;align-items:center;gap:12px}
.slider-head h2{font-size:20px;margin:0}
#velocidade{font-size:23px;font-weight:800;color:#dcefff;font-variant-numeric:tabular-nums}
.speed-meter{height:8px;border-radius:30px;background:#223146;overflow:hidden;margin:15px 0 9px}
#progress{height:100%;width:30%;background:linear-gradient(90deg,#488dea,#7fc3fb);border-radius:30px;transition:width .12s}
.speed-actions{display:flex;justify-content:space-between;gap:12px;align-items:center;margin-top:13px}
.speed-actions button{border:1px solid #3a5475;background:#1b314e;border-radius:10px;color:#d0e5fb;padding:12px 16px;font-weight:750;font-size:13px;cursor:pointer;min-height:44px}
.speed-actions button:hover{background:#26456d}
.ends{display:flex;justify-content:space-between;gap:10px;color:#8198b6;font-size:11px}
.actions{display:flex;justify-content:space-between;align-items:center;gap:18px;flex-wrap:wrap;margin-top:20px}
.status{font-size:13px;color:#83d9b7;font-weight:700}
.status.error{color:#ffadb9}
.stop{background:#cf3451;border:1px solid #fa7a93;border-radius:13px;padding:14px 25px;color:white;font-size:14px;font-weight:900;cursor:pointer;min-width:168px}
button:focus-visible,.pad:focus-visible{outline:3px solid #ffd378;outline-offset:4px}
footer{margin-top:18px;color:#8495ae;font-size:12px;line-height:1.7;text-align:center}
@media(max-width:720px){main{padding:17px 12px 36px}.top{grid-template-columns:1fr}.panel{padding:18px}.joystick-panel{min-height:318px}.pad{width:220px;height:220px}.actions{align-items:stretch}.stop{width:100%}.settings{gap:15px}}
@media(prefers-reduced-motion:reduce){#progress{transition:none}}
</style>
</head>
<body>
<main>
<header>
 <div><p class="eyebrow">ESP32 · WI-FI · ETAPA 04</p><h1>Painel do Carrinho</h1><p class="subtitle">Joystick, velocidade e iluminação em uma única tela</p></div>
 <span class="pill" id="modeLabel">Verificando...</span>
</header>
<div class="notice" id="notice" role="note">Preparando controle...</div>
<div class="top">
 <section class="panel joystick-panel" aria-labelledby="joystick-title">
  <h2 id="joystick-title">Joystick virtual</h2>
  <div class="pad" id="pad" tabindex="0" role="group" aria-label="Joystick: arraste para mover ou use as setas do teclado; solte para parar">
   <div class="vertical" aria-hidden="true"></div><div class="knob" id="knob" aria-hidden="true">✥</div>
  </div>
  <div class="joystate"><span class="dot" aria-hidden="true"></span><span id="direction">Parado</span></div>
  <div class="small">Arraste para dirigir · solte para parar<br>Ou utilize as setas do teclado</div>
 </section>
 <section class="panel settings" aria-labelledby="settings-title">
  <div>
    <p class="field-top">RECURSOS DO CARRINHO</p>
    <h2 id="settings-title" style="margin:7px 0 0">Faróis e sinalização</h2>
  </div>
  <div class="feature">
    <div class="feature-head"><strong>◉ Faróis por sensor de luz</strong><span class="feature-badge">Automático</span></div>
    <p>O ESP32 lê o sensor LDR e acende os LEDs quando o ambiente fica escuro. Ao virar, os LEDs piscam em laranja.</p>
  </div>
  <div class="step-list">
   <div class="step"><i>01</i><span>Conecte-se ao Wi-Fi do carrinho</span></div>
   <div class="step"><i>02</i><span>Movimente o joystick para escolher a direção</span></div>
   <div class="step"><i>03</i><span>Solte o joystick ou toque em PARAR</span></div>
  </div>
  <p class="small" style="text-align:left">Nesta etapa, o joystick usa quatro direções básicas. O movimento proporcional fica na versão integrada.</p>
 </section>
</div>
<section class="panel bottom" aria-labelledby="speed-title">
 <div class="slider-head"><h2 id="speed-title">Velocidade máxima</h2><output id="velocidade" aria-live="polite">30%</output></div>
 <div class="speed-meter" role="img" aria-label="Indicador visual da velocidade"><div id="progress"></div></div>
 <div class="ends"><span>30% · mais lento</span><span>100% · potência calibrada</span></div>
 <div class="speed-actions">
   <button id="menos" type="button">− Desacelerar</button>
   <button id="mais" type="button">+ Acelerar</button>
 </div>
 <div class="actions">
  <div class="status" id="status" role="status" aria-live="polite">Parado</div>
  <button type="button" class="stop" id="stop">■ &nbsp; PARAR</button>
 </div>
</section>
<footer>Esta página fora do ESP32 é apenas uma simulação; não envia comandos para motores. Para controlar o carrinho físico, conecte-se à rede local dele e abra <strong>http://192.168.4.1/</strong>. Teste primeiro com as rodas suspensas e tenha acesso ao corte de energia. PARAR não é um desligamento físico.</footer>
</main>
<script>
(() => {
  "use strict";
  const $ = id => document.getElementById(id);
  const local = location.hostname === "192.168.4.1";
  const pad = $("pad"), knob = $("knob"), status = $("status");
  let velocidade = 30, comando = "parar", pointer = null, repeticao = null;
  let pendente = null, enviando = false;
  $("modeLabel").textContent = local ? "● Rede local · ESP32" : "◉ Demonstração · Web";
  $("notice").textContent = local
    ? "Controle local: os comandos são enviados por Wi-Fi para o ESP32."
    : "Modo demonstração: experimente o joystick e os ajustes sem movimentar nenhum motor.";

  // A pagina usa somente a rota /comando do firmware simples da etapa 04.
  async function enviarFila() {
    if (enviando || !local) return;
    enviando = true;
    try {
      while (pendente !== null) {
        const proximo = pendente;
        pendente = null;
        try {
          const resposta = await fetch("/comando?valor=" + encodeURIComponent(proximo), {cache:"no-store"});
          if (!resposta.ok) throw new Error("HTTP " + resposta.status);
          status.classList.remove("error");
        } catch (erro) {
          status.classList.add("error");
          status.textContent = "Falha na comunicação: " + erro.message;
        }
      }
    } finally { enviando = false; }
  }
  function enviar(valor) { pendente = valor; enviarFila(); }
  function visualizar(direcao, px = 0, py = 0) {
    comando = direcao;
    const raio = (pad.clientWidth - knob.clientWidth)/2;
    knob.style.transform = "translate(" + (px*raio) + "px," + (py*raio) + "px)";
    $("direction").textContent = direcao === "parar" ? "Parado" : direcao.charAt(0).toUpperCase()+direcao.slice(1);
    if (!status.classList.contains("error")) status.textContent =
      (local ? "Movimento: " : "Simulação: ") + (direcao === "parar" ? "parado" : direcao);
  }
  function definirDirecao(direcao, px, py) {
    if (comando !== direcao) enviar(direcao);
    visualizar(direcao, px, py);
  }
  function apontar(e) {
    const rect = pad.getBoundingClientRect();
    const raio = (rect.width - knob.clientWidth)/2;
    let px = e.clientX - rect.left - rect.width/2;
    let py = e.clientY - rect.top - rect.height/2;
    const dist = Math.hypot(px, py);
    if (dist > raio) { px *= raio/dist; py *= raio/dist; }
    const normalX = px/raio, normalY = py/raio;
    let dir = "parar";
    if (dist > 25) dir = Math.abs(py) >= Math.abs(px) ?
      (py < 0 ? "frente" : "re") : (px < 0 ? "esquerda" : "direita");
    definirDirecao(dir, normalX, normalY);
  }
  function parar() {
    clearInterval(repeticao); repeticao = null; pointer = null;
    visualizar("parar");
    enviar("parar");
  }
  pad.addEventListener("pointerdown", e => {
    if (pointer !== null) return;
    e.preventDefault();
    pointer = e.pointerId;
    pad.setPointerCapture(e.pointerId);
    apontar(e);
    repeticao = setInterval(() => { if (comando !== "parar") enviar(comando); }, 250);
  });
  pad.addEventListener("pointermove", e => { if (pointer === e.pointerId) apontar(e); });
  ["pointerup","pointercancel","lostpointercapture"].forEach(evento =>
    pad.addEventListener(evento, () => { if (pointer !== null) parar(); })
  );
  const teclas = {ArrowUp:["frente",0,-1],ArrowDown:["re",0,1],
    ArrowLeft:["esquerda",-1,0],ArrowRight:["direita",1,0]};
  pad.addEventListener("keydown", e => {
    if (!teclas[e.key]) return;
    e.preventDefault();
    const [dir, x, y] = teclas[e.key];
    if (repeticao === null) repeticao = setInterval(() => { if(comando !== "parar") enviar(comando); }, 250);
    definirDirecao(dir, x, y);
  });
  pad.addEventListener("keyup", e => { if (teclas[e.key]) { e.preventDefault(); parar(); } });
  pad.addEventListener("blur", parar);
  $("stop").addEventListener("click", parar);
  window.addEventListener("blur", parar);
  document.addEventListener("visibilitychange", () => { if (document.hidden) parar(); });
  function velocidadeNova(delta) {
    const nova = Math.max(30, Math.min(100, velocidade + delta));
    if (nova === velocidade) return;
    velocidade = nova;
    $("velocidade").textContent = velocidade + "%";
    $("progress").style.width = velocidade + "%";
    enviar(delta > 0 ? "acelerar" : "desacelerar");
  }
  $("menos").addEventListener("click", () => velocidadeNova(-10));
  $("mais").addEventListener("click", () => velocidadeNova(10));
})();
</script>
</body>
</html>
)ETAPA04HTML";

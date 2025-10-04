<!DOCTYPE html>
<html lang="pl">
<head>
  <meta charset="UTF-8">
  <title>Wciągarka – pozycja liny</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #f4f4f4;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 40px;
    }

    .container {
      display: flex;
      align-items: flex-start;
      gap: 30px;
    }

    .track-wrapper {
      position: relative;
      width: 50px;
      height: 420px;
      background: #ddd;
      border-radius: 10px;
      overflow: hidden;
      box-shadow: inset 0 0 5px #aaa;
    }

    .actual-fill {
      position: absolute;
      top: 0;
      width: 100%;
      background: #4caf50;
      transition: height 0.3s ease;
    }

    .floating-label-left {
      position: absolute;
      left: 50%;
      transform: translate(-50%, -50%);
      font-size: 14px;
      background: black;
      color: white;
      padding: 6px 10px;
      border-radius: 6px;
      white-space: nowrap;
      pointer-events: none;
      z-index: 2;
      box-shadow: 0 2px 4px rgba(0,0,0,0.3);
    }

    .target-indicator {
      position: absolute;
      width: 100%;
      height: 5px;
      background: red;
      left: 0;
      transform: translateY(-50%);
    }

    .slider-container {
      position: relative;
      height: 420px;
    }

    .slider {
      writing-mode: bt-lr;
      -webkit-appearance: slider-vertical;
      width: 30px;
      height: 100%;
      transform: scaleY(-1);
    }

    .floating-label-right {
      position: absolute;
      left: calc(100% + 10px);
      font-size: 14px;
      background: black;
      color: white;
      padding: 6px 10px;
      border-radius: 6px;
      transform: translateY(-50%);
      white-space: nowrap;
      pointer-events: none;
      box-shadow: 0 2px 4px rgba(0,0,0,0.3);
    }

    .controls {
      margin-top: 30px;
      display: flex;
      flex-wrap: wrap;
      gap: 30px;
    }

    label {
      font-weight: bold;
    }

    input[type="number"] {
      width: 80px;
      padding: 4px;
      font-size: 14px;
    }
  </style>
</head>
<body>

  <h2>Pozycja liny wciągarki</h2>
  <div class="container">
    <div class="track-wrapper">
      <div class="actual-fill" id="actualFill"></div>
      <div class="floating-label-left" id="labelAktualna">0.0 m</div>
      <div class="target-indicator" id="targetIndicator"></div>
    </div>

    <div class="slider-container">
      <input type="range" min="0" max="14" step="0.1" value="0" class="slider" id="sliderDocelowa">
      <div class="floating-label-right" id="labelDocelowa">0.0 m</div>
    </div>
  </div>

  <div class="controls">
    <label>Aktualna pozycja [m]:
      <input type="number" id="inputAktualna" min="0" max="14" step="0.1" value="0">
    </label>

    <label>Docelowa pozycja [m]:
      <input type="number" id="inputDocelowa" min="0" max="14" step="0.1" value="0">
    </label>
  </div>

  <script>
    const MAX_POZYCJA = 14;
    const trackHeight = 420;

    const slider = document.getElementById('sliderDocelowa');
    const inputAktualna = document.getElementById('inputAktualna');
    const inputDocelowa = document.getElementById('inputDocelowa');
    const actualFill = document.getElementById('actualFill');
    const targetIndicator = document.getElementById('targetIndicator');
    const labelAktualna = document.getElementById('labelAktualna');
    const labelDocelowa = document.getElementById('labelDocelowa');

    function clamp(val, min, max) {
      return Math.max(min, Math.min(max, val));
    }

    function updateDisplay() {
      const aktualna = parseFloat(inputAktualna.value) || 0;
      const docelowa = parseFloat(slider.value) || 0;

      const actualY = (aktualna / MAX_POZYCJA) * trackHeight;
      const targetY = (docelowa / MAX_POZYCJA) * trackHeight;

      actualFill.style.height = `${actualY}px`;

      labelAktualna.innerText = `${aktualna.toFixed(1)} m`;
      labelAktualna.style.top = `${clamp(actualY, 10, trackHeight - 10)}px`;

      labelDocelowa.innerText = `${docelowa.toFixed(1)} m`;
      labelDocelowa.style.top = `${clamp(targetY, 10, trackHeight - 10)}px`;

      // tylko jeśli użytkownik nie edytuje pola inputDocelowa – unikamy anomalii!
      if (document.activeElement !== inputDocelowa) {
        inputDocelowa.value = docelowa.toFixed(1);
      }

      targetIndicator.style.top = `${targetY}px`;
    }

    slider.addEventListener('input', updateDisplay);
    inputAktualna.addEventListener('input', updateDisplay);

    inputDocelowa.addEventListener('input', () => {
      const value = parseFloat(inputDocelowa.value);
      if (!isNaN(value)) {
        slider.value = clamp(value, 0, MAX_POZYCJA);
        updateDisplay();
      }
    });

    updateDisplay();
  </script>

</body>
</html>

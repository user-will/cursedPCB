class CursedCard extends HTMLElement {
  connectedCallback() {
    this.innerHTML = `
      <div class="topline">
        <span>Cursed PCB</span>
        <span>ATtiny412 / setup</span>
      </div>

      <section class="alert">
        <div class="stop-sign" aria-label="Stop">STOP</div>
        <div>
          <h1>Set up Arduino<br><em>before uploading.</em></h1>
          <p>Use a USB-C <b>data</b> cable.</p>
        </div>
      </section>

      <section class="step">
        <div class="num">01</div>
        <div>
          <h2>Install megaTinyCore</h2>
          <p>Arduino IDE → Preferences → <b>Additional Boards Manager URLs:</b></p>
          <div class="package-url">http://drazzy.com/<br>package_drazzy.com_index.json</div>
          <p>Boards Manager: install <b>megaTinyCore</b> by Spence Konde.<br><b class="library-inline">LED library included.</b></p>
        </div>
      </section>

      <section class="step">
        <div class="num">02</div>
        <div>
          <h2>Choose these settings</h2>
          <div class="settings">
            <div>Board</div><div>ATtiny412/402/212/202<br><b>without Optiboot</b></div>
            <div>Clock</div><div>20 MHz internal</div>
            <div>BOD voltage</div><div><b>4.2V (20 MHz or less)</b></div>
            <div>Programmer</div><div>SerialUPDI – 230400 baud</div>
          </div>
        </div>
      </section>

      <section class="step">
        <div class="num">03</div>
        <div>
          <h2>Upload the example</h2>
          <p>Open <b>LED_example.ino</b>. Choose <b>Sketch → Upload Using Programmer</b>.</p>
        </div>
      </section>

      <div class="power">Power the board using the side <b>VCC</b> pin with up to <b>20V</b>. Supplying raw voltage to the <b>5V</b> pin may cause damage.</div>

      <section class="qr-band">
        <img src="images/cursed-pcb-github-qr.png" alt="QR code for the Cursed PCB guide and code">
        <div>
          <div class="qr-label">Full guide + example code</div>
          <strong>github.com/user-will/<br>cursedPCB</strong>
          <svg class="brand-logo" viewBox="243 276 1615 468" role="img" aria-label="Reil Industrial">
            <title>Reil Industrial</title>
            <image href="images/reil_industrial_logo.png" width="2137" height="1100"></image>
          </svg>
        </div>
      </section>`;
  }
}

customElements.define("cursed-card", CursedCard);

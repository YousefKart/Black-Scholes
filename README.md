<h1>Black-Scholes Visualizer (C++)</h1>
<p>An interactive desktop application for visualizing European-style option pricing using the Black-Scholes model.</p>
<p>Built with C++17 and Qt 6, this tool allows real-time exploration of how option prices change with respect to stock price, volatility, time to expiry, and more.</p>

<img width="702" height="536" alt="image" src="https://github.com/user-attachments/assets/af6f5e7e-20cb-4e7d-99df-e2329ed72ebd" />
<img width="702" height="536" alt="image" src="https://github.com/user-attachments/assets/658a3da7-8f92-471e-9eb3-061fb046b567" />

<hr>

<h2>Features</h2>
<ul>
  <li>Interactive Heatmap surface</li>
  <li>Real-time parameter updates</li>
  <li>Toggle between Call and Put prices</li>
  <li>Multiple surface modes:
  <ul>
    <li><code>(S,T) -> Price</code></li>
    <li><code>(S,σ) -> Price</code></li>
    <li><code>(S,σ) -> Delta</code> (Coming soon)</li>
  </ul>
  </li>
  <li>Ckean MVC-style separation:
  <ul>
    <li><code>Component.cpp</code> -> UI Layer</li>
    <li><code>Compute.cpp</code> -> Pricing Engine</li>
    <li><code>Functions.cpp</code> -> Math Foundation</li>
    <li><code>Surface.cpp</code> -> Surface Configuration</li>
  </ul>
  </li>
</ul>

<hr>

<h2>Black–Scholes Formula</h3>

<h3>1. d₁</h4>

$$
\Large d_1 = \frac{\ln\left(\frac{S}{K}\right) + \left(r - q + \frac{1}{2}\sigma^2\right)T}{\sigma \sqrt{T}}
$$

<h3>2. d₂</h4>

$$
\Large d_2 = d_1 - \sigma \sqrt{T}
$$

<h3>3. Standard Normal CDF</h3>

$$
\Large N(x) = \frac{1}{\sqrt{2\pi}} \int_{-\infty}^{x} e^{-\frac{t^2}{2}} \, dt
$$

<h3>4. Option Prices</h4>

<p><strong>Call Option</strong></p>

$$
\Large C = S e^{-qT} N(d_1) - K e^{-rT} N(d_2)
$$

<p><strong>Put Option</strong></p>

$$
\Large P = K e^{-rT} N(-d_2) - S e^{-qT} N(-d_1)
$$


<h3>Legend</h3>
<ul>
  <li>S = Stock Price</li>
  <li>K = Strike Price</li>
  <li>r = Risk-Free Rate</li>
  <li>q = Dividend Yield</li>
  <li>σ = Volatility</li>
  <li>T = Time to Expiry</li>
</ul>

<hr>

<h2>Build Instructions</h2>
<h3>Requirements</h3>
<ul>
  <li>Qt 6.5+</li>
  <li>C++17</li>
  <li>CMake 3.16+</li>
  <li>MinGW or MSVC</li>
</ul>
<h3>Build</h3>

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

<hr>

<h2>Roadmap</h2>

- [x] Add <code>(S,T) -> Price</code> Surface
- [x] Add <code>(S,σ) -> Price</code> Surface
- [ ] Add <code>(S,σ) -> Delta</code> Surface
- [ ] Add <code>Gamma</code> Surfaces
- [ ] Add Implied Volatility Solver
- [ ] GPU Acceleration (OpenGL Surface)
- [ ] Export Surface as Image

<hr>

<h2>Author</h2>
<p>Created by <strong>Yousef Kart</strong></p>

<hr>

<h2>License</h2>
<p>MIT License</p>

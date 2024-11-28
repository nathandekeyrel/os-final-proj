<h1>Analysis of Scheduling Algorithms</h1>

<h2>CSCI - 460 Final Project</h2>
<p>We compared various algorithms seen both inside and outside of class</p>
<p>In class, we covered</p>
<ul>
  <li>First Come First Serve</li>
  <li>Round Robin</li>
  <li>Shortest Process Next</li>
  <li>Shortest Remaining Time</li>
  <li>Highest Response Ratio Next</li>
  <li>Feedback</li>  
</ul>
<p>While we wanted to cover things we have already seen, we wanted to see what's used in modern operating systems</p>
<p>This led us to structure our project as follows:</p>
<ol>
  <li>Basic algorithms:
    <ul>
      <li>First Come First Serve</li>
      <li>Round Robiin</li>
    </ul>
  </li>
  <li>Theoretically optimal algorithms:
    <ul>
      <li>Shortest Process Next</li>
      <li>Shortest Remaining Time</li>
    </ul>
  </li>
</ol>
<p>While looking for new algorithms, we came across the Completely Fair Scheduler used by Linux</p>
<h3>CFS</h3>
<p>This algorithm uses a Red Black Tree to schedule processes. It:</p>
<ul>
  <li>(will put more in this readme later)</li>
</ul>

<p>There were two aspects of the project developed in python: Workload generation and metric visualization.</p>
<p>Working with files and graphs are just easier in python.</p>

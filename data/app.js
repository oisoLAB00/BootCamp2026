let labels = [];
let emg1Data = [];
let emg2Data = [];
let fsr1Data = [];
let fsr2Data = [];

const ctx = document.getElementById('chart').getContext('2d');

const chart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [
            {
                label: 'EMG1',
                data: emg1Data
            },
            {
                label: 'EMG2',
                data: emg2Data
            },
            {
                label: 'FSR1',
                data: fsr1Data
            },
            {
                label: 'FSR2',
                data: fsr2Data
            }
        ]
    },
    options: {
        animation: false
    }
});

async function updateSensor() {
    const response = await fetch('/sensor');
    const data = await response.json();

    //document.getElementById("mode").innerText = data.mode;
    if(data.mode == 0){
        document.getElementById("mode").innerText = "STOP";
    }
    else if(data.mode == 1){
        document.getElementById("mode").innerText = "CALIB";
    }
    else if(data.mode == 2){
        document.getElementById("mode").innerText = "WORK";
    }    
    let count = 0;  
    labels.push(count++);

    emg1Data.push(data.emg1);
    emg2Data.push(data.emg2);
    fsr1Data.push(data.fsr1);
    fsr2Data.push(data.fsr2);

    if(labels.length > 100){
        labels.shift();
        emg1Data.shift();
        emg2Data.shift();
        fsr1Data.shift();
        fsr2Data.shift();
    }

    chart.update();
}

async function startCalib(){
    await fetch('/start_calib');
}

async function setStop(){
    await fetch('/stop');
}

async function setWork(){
    await fetch('/work');
}

async function updateThreshold(){
    let th = document.getElementById("threshold").value;
    await fetch(`/threshold?value=${th}`);
}

setInterval(updateSensor, 50);
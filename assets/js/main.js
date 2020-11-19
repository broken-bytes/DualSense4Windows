
function GetDeviceNode(path, name, mode, color) {
  let column = document.createElement("div");
  column.className = "row align-items-start";

  let cPath = document.createElement("div");
  cPath.className = "col"
  let pPath = document.createElement("p");
  pPath.innerHTML = path;
  cPath.appendChild(pPath);

  let cName = document.createElement("div");
  cName.className = "col";
  let pName = document.createElement("p");
  pName.innerHTML = name;
  cName.appendChild(pName);

  let cMode = document.createElement("div");
  cMode.className = "col";
  let pMode = document.createElement("p");
  pMode.innerHTML = mode;
  cMode.appendChild(pMode);


  let cColor = document.createElement("div");
  cColor.className = "col";
  let pColor = document.createElement("p");
  pColor.innerHTML = color;
  cColor.appendChild(pColor);

  column.appendChild(cPath);
  column.appendChild(cName);
  column.appendChild(cMode);
  column.appendChild(cColor);
  return column;
}

function OnDualSenseDevicesUpdated(devices) {
  let deviceData = document.getElementById('device-data');
  devices.devices.forEach((device, i) => {
    deviceData.appendChild(GetDeviceNode(i, "DualSense", device.device, "Green"));
  });
}

function Init() {
}

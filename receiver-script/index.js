import SerialPort from "serialport";
import Readline from "@serialport/parser-readline";
import fs from "fs";

const csvPath = "<yourFilePath>.csv";
const serialPort = "<yourArduinoSerialPort>";

const parser = new Readline();

const port = new SerialPort(serialPort, {
  baudRate: 9600,
});

port.pipe(parser);

fs.stat(csvPath, (err, stats) => {
  console.log(JSON.stringify(err));
  if (err && err.errno === -4058) {
    fs.writeFileSync(csvPath, "work,study,play\n");
  } else if (!!stats) {
    if (fs.readFileSync(csvPath).length === 0) {
      fs.appendFileSync(csvPath, "work,study,play\n");
    }
  }
});

parser.on("data", (data) => {
  const parsed = JSON.parse(data);
  const cleaned = Object.keys(parsed).reduce((acc, curr) => {
    acc[curr] = Math.ceil(parsed[curr] / 1000);
    return acc;
  }, {});

  const toAppend = `${Object.values(cleaned).join(",")}\n`;
  fs.appendFileSync(csvPath, toAppend);
});

var host = "broker.emqx.io";
var port = 8083;
var topic1 = "test/configMPU";
var topic2 = "test/MPU"


// Create mqtt client1 
var client = new Paho.MQTT.Client(host, port, "tan");

client.destinationName = topic1;
client.qos = 1;
client.retained = true;

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({onSuccess:onConnect});


// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client.subscribe(topic1);  

}

var count =0;
function publish_client1()
{
    var sample_data = document.getElementById("sample_txt").value;
    var DLPF_data = document.getElementById("DLPF_txt").value;
    var GYRO_data = document.getElementById("GYRO_txt").value;
    var accuracy_data = document.getElementById("accuracy_txt").value;
    if (sample_data == "" || DLPF_data == "" || GYRO_data == "" || accuracy_data == "")
      alert("Wrong format!");   
    else
    {
      if (sample_data <0|| sample_data > 255)
          alert("Wrong Sample Rate!");
      else if (DLPF_data <0|| DLPF_data > 7)
          alert("Wrong Digital Low Pass Filter!");
      else if (GYRO_data <0|| GYRO_data > 3)
          alert("Wrong GYRO Scale!");
       else if (GYRO_data <0|| GYRO_data > 3)
          alert("Wrong Accelerometer Scale");
      else{
          message = new Paho.MQTT.Message(sample_data +"/"+ DLPF_data +"/"+ GYRO_data+ "/0" + "/"+ accuracy_data);
          message.destinationName = topic1;
          message.qos = 1;
          console.log(message);
          client.send(message);
        }
    }
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {

    
    console.log("onMessageArrived from " + topic1 + ":" + message.payloadString);
}
//---- End client 1 -------//



// Create mqtt client2
var client2 = new Paho.MQTT.Client(host, port, "tan_2");

client2.destinationName = topic2;
client2.qos = 1;
client2.retained = false;

// set callback handlers
client2.onConnectionLost = onConnectionLost_2;
client2.onMessageArrived = onMessageArrived_2;

// connect the client2
client2.connect({ onSuccess: onConnect2 });


// called when the client2 connects
function onConnect2() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client2.subscribe(topic2);

}

var count2 = 0;
function publish_client2() {
  message = new Paho.MQTT.Message('{"Ax":30,"Ay":70,"Az":80}');
  count2++;
  message.destinationName = topic2;
  message.qos = 1;
  client2.send(message);
}

// called when the client2 loses its connection
function onConnectionLost_2(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived_2(message) {
    var rawData = message.payloadString;
    const obj = JSON.parse(rawData);

    document.getElementById("bienphp_Ax_id").value= obj.Ax;
    document.getElementById("bienphp_Ay_id").value= obj.Ay;
    document.getElementById("bienphp_Az_id").value= obj.Az;


    

    $(document).ready(function(){
      //code xu ly form
      //sql config  
      $(document).on('submit','#form',function(){
          $.post("addData_config.php",$('#form').serialize(), function(){
              // alert("data sent")
          });
          return false;
      });
      //sql pi
      $.post("addData_mpu.php",$('#ax_test').serialize(), function(){
          // alert("abc")
      });
    return false;
      // code update table
      
  });
    console.log("onMessageArrived from " + topic2 + ":" + message.payloadString);
}
//---- End client 2 -------//
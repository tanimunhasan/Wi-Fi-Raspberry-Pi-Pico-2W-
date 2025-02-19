function doPost(e) {
  // Log the entire event object to check what is being received
  Logger.log("Incoming request: " + JSON.stringify(e));  

  // Check if e.parameter exists and receivedFloat is present
  if (!e.parameter || !e.parameter.receivedFloat) {
    Logger.log("Error: Missing receivedFloat parameter");
    return ContentService.createTextOutput("Error: Missing receivedFloat parameter");
  }

  var receivedFloat = e.parameter.receivedFloat;
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
  sheet.appendRow([new Date(), receivedFloat]);

  return ContentService.createTextOutput("Data received successfully");
}

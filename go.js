let BNFT=require("../dawn_console/BNFT.js");
let fs=require("fs");
let exec=require("child_process").exec;
let fileToString = function(filename)
{
    return fs.readFileSync(filename, {option:'utf8', function(err, source) {console.log("error reading "+filename);throw err;}}).toString();
}
let uil_syntax = fileToString("uil.bnft");
let parser = new BNFT(uil_syntax,{alert:console.log});
if (parser)
{
    let result = parser.parse(fileToString("test.uil"),{alert:console.log});
    console.log(result);
    result = eval(result);
    fs.writeFileSync("result.html", result);
    exec("result.html");
}

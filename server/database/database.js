var MongoClient = require('mongodb').MongoClient
, assert = require('assert');
// Connection URL
// var url = "mongodb://192.168.10.9:27017/SignalDataBase";
var url = "mongodb://127.0.0.1:27017/local";
var logger;

function initialize(v_logger){
	logger = v_logger;
	return this;
}

function insert(data, v_sheet, callback){
	// Use connect method to connect to the Server
	MongoClient.connect(url, function(err, db) {
		assert.equal(null, err);
		// console.log("      DB: Connected correctly to server");
		var collection = db.collection(v_sheet);
		collection.insert(data, function(){
			callback();
			db.close();
		});
	});
}

function list(callback){
	// Use connect method to connect to the Server
	MongoClient.connect(url, function(err, db) {
		assert.equal(null, err);
		console.log("      DB: Connected correctly to server");
		findAll(db, function(){
			callback();
			db.close();
		});
	});
}

function clear(callback){
	// Use connect method to connect to the Server
	MongoClient.connect(url, function(err, db) {
		assert.equal(null, err);
		console.log("      DB: Connected correctly to server");
		var collection;
		collection = db.collection("HVAC");
		collection.remove({}, function(){
			callback("HVAC");
			db.close();
		});
		collection = db.collection("fixedprox");
		collection.remove({}, function(){
			callback("fixedprox");
			db.close();
		});
		collection = db.collection("mobileprox");
		collection.remove({}, function(){
			callback("mobileprox");
			db.close();
		});
		collection = db.collection("messages");
		collection.remove({type: "messages"}, function(){
			callback("messages");
			db.close();
		});
		// collection = db.collection("HVAC");
		// collection.remove({});
	});
}

function insertData(db, data, callback){
	// Get the documents collection
	var collection = db.collection('documents');
	// Insert some documents
	collection.insertMany(data, {upsert: true}, function(err, result) {
		var len=data.length;
		assert.equal(null, err);
		assert.equal(data.length, result.insertedCount);
		console.log("      DB: Inserted " + len + " documents into the document collection");
		callback(result);
	});
}

function findAll(db, callback) {
	var collection = db.collection('documents');
	collection.find({}).toArray(function(err, docs) {
		assert.equal(err, null);
		assert.equal(docs.length, docs.length);
		console.log("      DB: Found the following records");
		console.dir(docs)
		callback(docs);
	});
}

function clearAll(db, callback){
	var collection = db.collection('documents');
	collection.deleteMany({}, function(err, r){
		assert.equal(null, err);
		callback(r);
	});
}

function query(v_sheet, v_conditions, callback){
	MongoClient.connect(url, function(err, db) {
		assert.equal(null, err);
		// console.log("      DB: Connected correctly to server");
		var collection;
		collection = db.collection(v_sheet);
		collection.find(v_conditions.condition, v_conditions.return).toArray(function(err, docs) {
			assert.equal(err, null);
			// console.log("      DB: The following records are found");
			// console.log(docs);
			callback(docs);
			db.close();
		});
	});
}

function aggregate(v_sheet, v_conditions, callback){
	MongoClient.connect(url, function(err, db) {
		assert.equal(null, err);
		// console.log("      DB: Connected correctly to server");
		var collection = db.collection(v_sheet);
		collection.aggregate(v_conditions).toArray(function(err, docs) {
			assert.equal(err, null);
			// console.log("      DB: The following records are found");
			// console.log(docs);
			callback(docs);
			db.close();
		});
	});
}

function update(v_sheet, v_conditions, v_modify, callback){
	MongoClient.connect(url, function(err, db) {
		assert.equal(null, err);
		// console.log("      DB: Connected correctly to server");
		var collection = db.collection(v_sheet);
		collection.find(v_conditions).forEach(function(v_d){
			v_modify(collection, v_d);
		}).toArray(function(err, docs) {
			assert.equal(err, null);
			// console.log("      DB: The following records are found");
			// console.log(docs);
			callback(docs);
			db.close();
		});
	});
}

module.exports = {
	initialize: initialize,
	query: query,
	aggregate: aggregate,
	update: update,
	insert: insert,
	list: list,
	clear: clear
};
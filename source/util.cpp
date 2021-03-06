#include "shared.h"
#include <jansson.h>

std::vector<char *> split(char * string, char * delim)
{
	std::vector<char *> array;

	char * token = strtok(string, delim);

	while (token != NULL)
	{
	    array.push_back(token);

	    token = strtok(NULL, delim);
	}

	return array;
}

void downloadFile(char * url, char * filename)
{
	httpcContext httpContext;

	Result returnResult = 0;

	u8 * buffer;

	u32 statusCode;

	u32 size;

	u32 bufferSize = 1024;

	returnResult = httpcOpenContext(&httpContext, HTTPC_METHOD_GET, url, 1);
	
	if (returnResult != 0) displayError("Failed to request url.");

	returnResult = httpcSetSSLOpt(&httpContext, SSLCOPT_DisableVerify);

	if (returnResult != 0) displayError("Failed to Disable SSL Verification.");

	char httpHeaderField[20];

	strcpy(httpHeaderField, "Flask/");
	strcat(httpHeaderField, flaskVersion);

	httpcAddRequestHeaderField(&httpContext, (char *)"User-Agent", httpHeaderField); 

	returnResult = httpcBeginRequest(&httpContext);

	if (returnResult != 0) displayError("Failed to begin http:C request");

	returnResult = httpcGetResponseStatusCode(&httpContext, &statusCode, 0);

	if (statusCode != 200) displayError("Invalid status code");

	buffer = (u8 *)malloc(bufferSize);

	if (buffer == NULL) displayError("Could not malloc httpc buffer.");

	char * sdmcPath = "sdmc:/flask/";

	char * fullpath = (char *)malloc(strlen(sdmcPath) + strlen(filename) + 1);

	strcpy(fullpath, sdmcPath);

	strcat(fullpath, filename);

	FILE * downloadedFile = fopen(fullpath, "w+");

	if (downloadedFile != NULL)
	{
		remove(fullpath);
	}

	do {
		
		returnResult = httpcDownloadData(&httpContext, buffer, bufferSize, &size);

		fwrite(buffer, 1, size, downloadedFile);

	} while (returnResult == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);


	if (returnResult != 0) displayError("Downloading file failed somehow");

	fflush(downloadedFile);

	fclose(downloadedFile);

	httpcCloseContext(&httpContext);
	
	free(buffer);

	free(fullpath);
}

Image * generateApplicationIcon(void) //temporary thing
{
	sf2d_texture * noneImage = sfil_load_PNG_file("graphics/none.png", SF2D_PLACE_RAM);

	Image * applicationIcon = new Image(noneImage);

	return applicationIcon;
}

void cacheData()
{
	downloadFile("https://api.titledb.com/v1/", "flask.json");

    json_t * flaskJson = json_load_file("sdmc:/flask/flask.json", JSON_DECODE_ANY, NULL);
 	
 	size_t arrayIndex;
	json_t * arrayValue;

	const char * objectKey;
	json_t * objectValue;

	json_array_foreach(flaskJson, arrayIndex, arrayValue) {
	    json_t * jsonObject = json_array_get(flaskJson, arrayIndex);

	    json_object_foreach(jsonObject, objectKey, objectValue) {
	  		Application temp(24, 24 + (arrayIndex * 64));

	  		if (strcmp(objectKey, "name") == 0)
	  		{
	  			temp.setName(json_string_value(objectValue));
	  		}
	  		else if (strcmp(objectKey, "author") == 0)
	  		{
	  			temp.setAuthor(json_string_value(objectValue));	
	  		}
	  		else if (strcmp(objectKey, "description") == 0)
	  		{
	  			temp.setDescription(json_string_value(objectValue));
	  		}
	  		else if (strcmp(objectKey, "url") == 0)
	  		{
	  			temp.setDownloadURL(json_string_value(objectValue));
	  		}

	  		//because we're not doing icons yet with cia stuff. .
	  		temp.setIcon(generateApplicationIcon());

	  		applications->push_back(temp);
		}
	}

	setScene(SC_FLASK);
}

int fsize(const char * file)
{
	struct stat st;

  	stat(file, &st);
  
  	return st.st_size;
}

void strstor(char * destination, const char * source)
{
	destination = (char *)malloc(strlen(source) + 1);

	strcpy(destination, source);
}

void loadBackgroundSong(void * arg)
{
	OggVorbis * backgroundMusic = new OggVorbis("audio/bgm.ogg");
	backgroundMusic->setLooping(true);
	backgroundMusic->setVolume(0.35);
	backgroundMusic->play();
}
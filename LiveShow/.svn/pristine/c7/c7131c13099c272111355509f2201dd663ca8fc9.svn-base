/*
  wrapper around libisomedia
*/

#include <string.h>
#include <assert.h>

#include "FFR_bitbuffer.h"
#include "mp4file.h"
#include "ISOMovies.h"
#include "MP4Movies.h"

#ifndef min
#define min(a, b)  ((a < b) ? (a) : (b))
#endif

#define MAX_COMPATIBLE_BRANDS 4

/*
 * datatypes
 */

struct PROFILE_AND_LEVEL {
  u8         OD;
  u8         scene;
  u8         audio;
  u8         visual;
  u8         graphics;
};


struct MP4_FILE {
  ISOMovie     mp4Movie;
  ISOTrack     mp4Track;
  ISOMedia     mp4Media;

  ISOHandle    hSampleEntry;
  ISOHandle    hSampleData;
  ISOHandle    hSampleDuration;
  ISOHandle    hSampleSize;
  ISOHandle    hDecoderSpecificInfo;

  unsigned int cBufferSizeByte;
  int          cNumFramesInPacket;

  int          bIsFirstFrame;
  int          frameCount;

  char*        fileName;

};


enum {
  MP4FF_NO_ERROR = 0
  ,MP4FF_ERROR_GENERAL_OPEN    = 100
  ,MP4FF_ERROR_OUT_OF_MEMORY
  ,MP4FF_ERROR_NO_FILENAME
  ,MP4FF_ERROR_OPEN_OUTPUTFILE

  ,MP4FF_ERROR_GENERAL_RUNTIME = 200
  ,MP4FF_ERROR_MP4FILE_NOT_OPEN
  ,MP4FF_ERROR_BUFFER_OVERFLOW
};

enum {
  ISO3GppR4Brand      = MP4_FOUR_CHAR_CODE( '3', 'g', 'p', '4' ),  /* brand for 3GPP release 4 files */
  ISO3GppR6Brand      = MP4_FOUR_CHAR_CODE( '3', 'g', 'p', '6' ),  /* brand for 3GPP release 6 files */
  ISO3Gpp2aBrand      = MP4_FOUR_CHAR_CODE( '3', 'g', '2', 'a' ),  /* brand for 3GPP2 files */
  ISOMpeg4V1Brand     = MP4_FOUR_CHAR_CODE( 'm', 'p', '4', '1' ),  /* brand for MPEG-4 version 1 */
  ISOMpeg4V2Brand     = MP4_FOUR_CHAR_CODE( 'm', 'p', '4', '2' )   /* brand for MPEG-4 version 2 */
};

/*
 * constants
 */

static const u32 BITRES_BITS  = 6144;
static const u32 BITRES_BYTES = 6144 / 8;

enum _AUDIO_OBJECT_TYPE {
  AOT_AAC_LC           = 2,
  AOT_SBR              = 5
};

static const unsigned int srIndexTable[12] = {96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000};

int
GetMPEG4ASConfig(unsigned int samplingRate,
                 unsigned int noChannels,
                 unsigned char* buf,
                 unsigned int*  numStreamMuxBits,
                 int bSbrPresent,
                 int bDingleRate) 
{ 
  struct BIT_BUF BitBuf;
  HANDLE_BIT_BUF hBitBuf = &BitBuf;
  unsigned char tmp[80];
  unsigned char* pBitBuf;
  unsigned int i, alignBits;
  unsigned int outBytes;
  unsigned int extensionSamplingRate;

  pBitBuf = &tmp[0];
  
  CreateBitBuffer(hBitBuf, pBitBuf, 80);

  /* audio specific config */
  WriteBits( hBitBuf, bSbrPresent ? AOT_SBR : AOT_AAC_LC, 5 );   /* AudioObjectType */

  for( i=0; i<sizeof(srIndexTable)/sizeof(unsigned int); i++ ) {
    if( srIndexTable[i] == samplingRate ) break;
  }
  WriteBits( hBitBuf, i, 4 );
  if( i == sizeof(srIndexTable)/sizeof(unsigned int) ) {
    WriteBits( hBitBuf, samplingRate, 24 );
  }

  if( (noChannels == 7) || (noChannels > 8) ) return -1;
  if( noChannels == 8 ) noChannels = 7; /* see : iso/mpeg4 v1 audio suppart1*/
  WriteBits( hBitBuf, noChannels, 4 );

  if (bSbrPresent) { /* we use only explicit, non-bc signalling */
    extensionSamplingRate = bDingleRate ? samplingRate : 2 * samplingRate;
    for( i=0; i<sizeof(srIndexTable)/sizeof(unsigned int); i++ ) {
      if( srIndexTable[i] == extensionSamplingRate ) break;
    }
    WriteBits( hBitBuf, i, 4 );
    if( i == sizeof(srIndexTable)/sizeof(unsigned int) ) {
      WriteBits( hBitBuf, extensionSamplingRate, 24 );
    }
    WriteBits( hBitBuf, AOT_AAC_LC, 5);
  }
  else {
    assert(0); /* not using SBR would need writing correct AAC-LC object type here */
  }

  /* GA specific config */
  WriteBits( hBitBuf, 0, 1);  /* frame length, signal 1024 */
  WriteBits( hBitBuf, 0, 1);  /* dependsOnCore */
  WriteBits( hBitBuf, 0, 1 ); /* Extension Flag */
 
  *numStreamMuxBits = GetBitsAvail(hBitBuf);
  outBytes = (*numStreamMuxBits+7)  >> 3;
  alignBits = (outBytes << 3) - *numStreamMuxBits;
  if (alignBits)
    WriteBits (hBitBuf, 0, alignBits);

  for (i=0; i<outBytes; i++)
    buf[i] = (unsigned char) ReadBits(hBitBuf,8);

  DeleteBitBuffer(&hBitBuf);

  return MP4FF_NO_ERROR;
}



/*
 * public functions
 */

/*!
  \brief   Opens a movie and prepares everything to simply add samples later
  \return  Error Code
*/
int
OpenMP4File(HANDLE_MP4_FILE* phMp4File,
            unsigned char* audioSpecConfigData,    /*!< audio spec. config data */
            int     audioSpecConfigByte,    /*!< size of audio spec. config in byte */
            char*   filename,               /*!< output filename */
            int     sampleRate,             /*!< aacPlus sampling frequency (incl. SBR) */
            int     bitRate,                /*!< aacPlus bitrate */
            int     nChannels,              /*!< number of AAC channels */
            int     bSbrPresent,            /*!< boolean, set TRUE if SBR is present */
            int     b3GppFile)              /*!< boolean, set TRUE if 3GPP brand shall be written */
{
  ISOErr     err  = ISONoErr;
  HANDLE_MP4_FILE hMp4;
  struct PROFILE_AND_LEVEL mp4ProfileLevel;
  FILE*      outfile = NULL;
  u32        initialObjectDescriptorID;
  u32        cFrameSize;
  u32        cDecBufferSize;
  u32        lenFileName;
  u32        majorBrand = ISO3GppR6Brand;
  u32        minorVersion = 0x0;
  u32        compatibleBrands[MAX_COMPATIBLE_BRANDS] = {0};
  int        i;

  assert(b3GppFile); /* nothing else supported here ! */

  *phMp4File = NULL; 

  hMp4 = (HANDLE_MP4_FILE)malloc( sizeof(struct MP4_FILE));
  if (hMp4==NULL)
    return MP4FF_ERROR_OUT_OF_MEMORY;

  hMp4->mp4Movie             = NULL;
  hMp4->mp4Track             = NULL;
  hMp4->mp4Media             = NULL;
  hMp4->hSampleEntry         = NULL;
  hMp4->hSampleData          = NULL;
  hMp4->hSampleDuration      = NULL;
  hMp4->hSampleSize          = NULL;
  hMp4->hDecoderSpecificInfo = NULL;
  hMp4->fileName             = NULL;

  /* initialize all ProfileAndLevels with 0xff -> no capability required */ 
  memset( &mp4ProfileLevel, 0xff, sizeof(struct PROFILE_AND_LEVEL));

  /* initialize correct audio profileLevelIndication and brands, no error checking applied
     libisomediafile automatically includes major brand and 'isom' into list of compatible brands */
  i = 0;
  if (bSbrPresent) {
    compatibleBrands[i++] = ISO3Gpp2aBrand;
    compatibleBrands[i++] = ISOMpeg4V2Brand;
    
    /* explicit hierarchical signalling */
    mp4ProfileLevel.audio = 0x2c;  /* HE AAC Profile L2 */
  } 
  else {
    assert(0); /* not using SBR would need writing correct profile & level here */
  }

  assert(i<=MAX_COMPATIBLE_BRANDS); /* check for max. number of compatible brands */

  initialObjectDescriptorID = 1;

  err = MP4NewMovie( &(hMp4->mp4Movie),
                     initialObjectDescriptorID,
                     mp4ProfileLevel.OD,
                     mp4ProfileLevel.scene,
                     mp4ProfileLevel.audio,
                     mp4ProfileLevel.visual,
                     mp4ProfileLevel.graphics);
  if (err != ISONoErr)
    return (int)err;

  err = ISOSetMovieBrand( hMp4->mp4Movie, majorBrand, minorVersion );
  
  if (err != ISONoErr)
    return (int)err;
  
  i = 0;
  while (i<MAX_COMPATIBLE_BRANDS && compatibleBrands[i]!=0) {
    err = ISOSetMovieCompatibleBrand( hMp4->mp4Movie, compatibleBrands[i]);
    if (err != ISONoErr)
      return (int)err;
    i++;
  }

  err = ISONewMovieTrack( hMp4->mp4Movie,
                          ISONewTrackIsAudio,
                          &(hMp4->mp4Track));
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }

  err = ISONewTrackMedia( hMp4->mp4Track,
                          &(hMp4->mp4Media),
                          ISOAudioHandlerType,  /* handler type */
                          sampleRate,           /* time scale */
                          NULL);                /* data reference */
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }

  err = ISOBeginMediaEdits( hMp4->mp4Media);
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }

  /* set language to undetermined */
  err = ISOSetMediaLanguage( hMp4->mp4Media, "und");
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }

  err = ISONewHandle( audioSpecConfigByte+6, &(hMp4->hDecoderSpecificInfo));
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }

  /* assemble decoder specific info */
  {
    u8* pDecoderSpecificInfo = (u8*) *hMp4->hDecoderSpecificInfo;
    u32 size;
    u32 mask;
    u32 head;
    int i;

    /* set DecoderSpecificInfo tag */
    *pDecoderSpecificInfo = 0x05;
    pDecoderSpecificInfo++;

    /* write length info */
    head = 0;
    for (i=21; i>0; i-=7) {
      mask = 0x7f << i;
      size = audioSpecConfigByte & mask;
      if (size || head) {
        head++;
        size = size >> i;
        size |= 0x80;
        *pDecoderSpecificInfo = size;
        pDecoderSpecificInfo++;
      }
    }
    mask = 0x7f;
    size = audioSpecConfigByte & mask;
    *pDecoderSpecificInfo = size;
    pDecoderSpecificInfo++;
    
    /* copy AudioSpecificConfig */
    memcpy(pDecoderSpecificInfo, audioSpecConfigData, audioSpecConfigByte);

    /* resize ISOHandle */
    head += 2;   /* add for tag and lowest 7 bits of length */
    ISOSetHandleSize( hMp4->hDecoderSpecificInfo, head+audioSpecConfigByte);
  }

  err = ISONewHandle( 0, &(hMp4->hSampleEntry));
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }

  cDecBufferSize=BITRES_BYTES*nChannels;
  err = MP4NewSampleDescription( hMp4->mp4Track,
                                 hMp4->hSampleEntry,
                                 1,
                                 0x40,             /* MPEG-4 Audio */
                                 0x05,             /* audio stream */
                                 cDecBufferSize,   /* decoder buffer size */
                                 bitRate,          /* max. bitrate */
                                 bitRate,          /* avg. bitrate */
                                 hMp4->hDecoderSpecificInfo);
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }

  /* SBR is present -> frame size is doubled */
  cFrameSize = (bSbrPresent) ? 2048 : 1024;
  /* 0.5s audio in 1 packet */
  hMp4->cNumFramesInPacket  = (sampleRate/2)/cFrameSize;
  /* buffer 0.5s audio payload + bitreservior, not more than 8KB per channel */
  hMp4->cBufferSizeByte = min(bitRate/2 + BITRES_BITS*nChannels + 7, (unsigned int)(65536*nChannels))/8;

  err = ISONewHandle( sizeof(u32), &(hMp4->hSampleDuration));
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }
  *((u32*) *(hMp4->hSampleDuration)) = cFrameSize;

  err = ISONewHandle( hMp4->cBufferSizeByte, &(hMp4->hSampleData));
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }
  ISOSetHandleSize( hMp4->hSampleData, 0);

  err = ISONewHandle( hMp4->cNumFramesInPacket*sizeof(u32), &(hMp4->hSampleSize));
  if (err != ISONoErr) {
    CloseMP4File(hMp4);
    return (int)err;
  }
  ISOSetHandleSize( hMp4->hSampleSize, 0);

  lenFileName = strlen(filename);
  if (lenFileName<1) {
    CloseMP4File(hMp4);
    return MP4FF_ERROR_NO_FILENAME;
  }

  /* testopen the outputfile */
  outfile = fopen(filename, "wb");
  if (!outfile) {
    CloseMP4File(hMp4);
    return MP4FF_ERROR_OPEN_OUTPUTFILE;
  }
  else {
    fclose( outfile);
  }

  hMp4->fileName = (char*)calloc( sizeof(char), lenFileName+1);
  if (hMp4->fileName==NULL) {
    CloseMP4File(hMp4);
    return MP4FF_ERROR_OUT_OF_MEMORY;
  }
  strncpy( hMp4->fileName, filename, lenFileName+1);


  hMp4->bIsFirstFrame = 1;
  hMp4->frameCount    = 0;

  *phMp4File = hMp4;

  return MP4FF_NO_ERROR;
}

/*!
  \brief   Add a single frame to the MPEG-4 file
  \return  Error Code
*/
int
MP4FileAddFrame(HANDLE_MP4_FILE mp4File,    /*!< MPEG-4 file handle */
                unsigned int*  payload,     /*!< single frame to add */
                unsigned int    numBytes)   /*!< payload size in bytes */
{
  ISOErr     err  = ISONoErr;
  u32*       pSampleSize;
  u8*        pSampleData;
  u32        sampleDataLen;

  /* zero bytes -> no frame -> but no error */
  if (numBytes==0)
    return MP4FF_NO_ERROR;

  /* check if mp4File is open */
  if (!mp4File)
    return MP4FF_ERROR_MP4FILE_NOT_OPEN;

  /* check if something is wrong */
  if (numBytes>mp4File->cBufferSizeByte)
    return MP4FF_ERROR_BUFFER_OVERFLOW;

  /* get number bytes in buffer */
  ISOGetHandleSize(mp4File->hSampleData, &sampleDataLen);

  /* check if we have enough frames for one packet or if data doesn't fit in buffer anymore */
  if (mp4File->frameCount >= mp4File->cNumFramesInPacket ||
      (mp4File->frameCount > 0 &&
       sampleDataLen+numBytes > mp4File->cBufferSizeByte)) {

    if (mp4File->bIsFirstFrame) {
      err = ISOAddMediaSamples( mp4File->mp4Media,
                                mp4File->hSampleData,      /* data for all samples */
                                mp4File->frameCount,       /* sample count */
                                mp4File->hSampleDuration,  /* sample duration */ 
                                mp4File->hSampleSize,      /* sample size */
                                mp4File->hSampleEntry,     /* sample entry */
                                NULL,                      /* composition offsets */
                                NULL);                     /* sync samples */
      mp4File->bIsFirstFrame = 0;
    }
    else {
      err = ISOAddMediaSamples( mp4File->mp4Media,
                                mp4File->hSampleData,
                                mp4File->frameCount,
                                mp4File->hSampleDuration,
                                mp4File->hSampleSize,
                                NULL,
                                NULL,
                                NULL);
    }
    if (err)
      return (int)err;

    err = ISOSetHandleSize(mp4File->hSampleData, 0);
    if (err)
      return (int)err;
    err = ISOSetHandleSize(mp4File->hSampleSize, 0);
    if (err)
      return (int)err;
    mp4File->frameCount = 0;
    sampleDataLen = 0;
  }

  /* append current frame to sampleData */
  pSampleData = (u8*)*(mp4File->hSampleData);
  pSampleData += sampleDataLen;
  sampleDataLen += numBytes;
  err = ISOSetHandleSize( mp4File->hSampleData, sampleDataLen);
  if (err)
    return (int)err;
  memcpy( pSampleData, payload, numBytes);

  /* update sampleSize with length of current frame */
  pSampleSize = (u32*)*(mp4File->hSampleSize);
  pSampleSize += mp4File->frameCount;
  err = ISOSetHandleSize( mp4File->hSampleSize, (mp4File->frameCount+1)*sizeof(u32));
  if (err)
    return (int)err;
  *pSampleSize = numBytes;

  mp4File->frameCount++;

  return (int)err;
}


/*!
  \brief   Flush buffers and write file
  \return  Error Code
*/
int
WriteMP4File(HANDLE_MP4_FILE mp4File)
{
  ISOErr     err  = ISONoErr;
  u64        mediaDuration;

  /* check if mp4File is open */
  if (!mp4File)
    return MP4FF_ERROR_MP4FILE_NOT_OPEN;

  /* flush the buffer */
  if (mp4File->frameCount)
    err = ISOAddMediaSamples( mp4File->mp4Media,
                              mp4File->hSampleData,
                              mp4File->frameCount,
                              mp4File->hSampleDuration,
                              mp4File->hSampleSize,
                              NULL,
                              NULL,
                              NULL);
  if (err)
    return (int)err;

  err = ISOGetMediaDuration( mp4File->mp4Media, &mediaDuration);
  if (err)
    return (int)err;

  err = ISOEndMediaEdits( mp4File->mp4Media);
  if (err)
    return (int)err;

  err = ISOInsertMediaIntoTrack( mp4File->mp4Track,
                                 0,                 /* track start time */
                                 0,                 /* media start time */
                                 mediaDuration,     /* segment duration */
                                 1);                /* media rate */
  if (err)
    return (int)err;

  err = ISOWriteMovieToFile( mp4File->mp4Movie,
                             mp4File->fileName);

  return (int)err;
}


/*!
  \brief   Frees memory
  \return  nothing
*/
void
CloseMP4File(HANDLE_MP4_FILE hMp4File)
{
  if (hMp4File) {
    if (hMp4File->fileName)
      free(hMp4File->fileName);
    if (hMp4File->hSampleEntry)
      ISODisposeHandle(hMp4File->hSampleEntry);
    if (hMp4File->hSampleData)
      ISODisposeHandle(hMp4File->hSampleData);
    if (hMp4File->hSampleDuration)
      ISODisposeHandle(hMp4File->hSampleDuration);
    if (hMp4File->hSampleSize)
      ISODisposeHandle(hMp4File->hSampleSize);
    if (hMp4File->hDecoderSpecificInfo)
      ISODisposeHandle(hMp4File->hDecoderSpecificInfo);

    if (hMp4File->mp4Movie)
      ISODisposeMovie(hMp4File->mp4Movie);

    free(hMp4File);
  }
}

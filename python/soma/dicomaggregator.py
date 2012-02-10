#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os, dicom

class DicomAggregator( object ):
    def __init__( self ):
        self._dicom_sources = set()

    def add_dicom_sources( self, sources ):
        for source in sources:
            self._dicom_sources.add( source )

    def aggregate( self ):
        self._aggregate_sources = {}
        serie_sop_uids = {}
        for source in self._dicom_sources:
            filelist = []
            if os.path.isdir( source ):
                for root, dirs, files in os.walk( source ):
                    for name in files:
                        filelist.append( os.path.join( root, name ) )
            else:
                filelist = [ source ]
            for f in filelist:
                try:
                    ds = dicom.read_file( f )
                except:
                    continue
                serie_uid = ds.SeriesInstanceUID
                if not self._aggregate_sources.has_key( serie_uid ):
                    self._aggregate_sources[ serie_uid ] = []
                    serie_sop_uids[ serie_uid ] = []

                sop_uid = ds.SOPInstanceUID
                if not sop_uid in serie_sop_uids[ serie_uid ]:
                    serie_sop_uids[ serie_uid ].append( sop_uid )
                    self._aggregate_sources[ serie_uid ].append( f )

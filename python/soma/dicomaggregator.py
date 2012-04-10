#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os, dicom

class DicomAggregator( object ):
    def __init__( self ):
        self._dicom_sources = []

    def add_dicom_sources( self, sources ):
        for source in sources:
            if os.path.exists(source):#os.path.isdir( source ):
                for root, dirs, files in os.walk( source ):
                    for name in files:
                        self._dicom_sources.append( os.path.join( root, name ) )
            else:
                self._dicom_sources.append( source )
        self._dicom_sources = list( set( self._dicom_sources ) )

    def aggregate( self ):
        self._aggregate_sources = {}
        serie_sop_uids = {}
        for f in self._dicom_sources:
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

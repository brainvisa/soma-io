=======
Soma IO
=======

A reimplementation of the former AIMS IO system, more generic, with a data-structure-independent API.

:somaiodox:`C++ doc <index.html>`

Soma-IO relies on :cartobdox:`Carto base <index.html>`: C++ general usage library.

Soma-IO has specializations, or implementations for specific data structures, like volumes etc, in :cartoddox:`Carto Data <index.html>`, and may have in other libraries in the future. Such specializations are generally available through :somaiodox:`plugins <classcarto_1_1Plugin.html>`.

See also the :somaiodox:`C++ docs for the IO system <soma_io.html>`

IO system URLs
==============

Options may be passed to the IO system through URL-like extensions to file names. The general syntax is similar to HTML requests:

::

    filename?option=x&other_option=y

All software using the Soma-IO system support such syntax. For instance you may run Anatomist to read a part of an image:

::

  anatomist my_mri.nii?ox=70&sx=100&oy=70&sy=150&oz=30&sz=100

The support for such options may depend on specific formats IO implementations. Some are generic to a data type (3D/4D volumes for instance), but may or may not be supported in a specific format.

Volume options
--------------

.. raw:: html

    <table cellpadding="2" cellspacing="0" class="docutils">
    <thead>
        <td>Option</td>
        <td>Type</td>
        <td>Explanation</td>
    </thead>
    <tr class="row-even">
        <td>ox, oy, oz, ot</td>
        <td>int</td>
        <td>
            Partial reading / writing origin in the volume on disk: correspond to the number of voxels to skip in x, y, z, t directions. x, y, z orientation is the coordinates system used in Soma-IO, currently fixed to the AIMS one (x: right-&gt;left, y: anterior-&gt;posterior, z: top-&gt;bottom). t is the time (4th) dimension.<br/>
            <b>Note</b> that for writing, partial IO must be enabled through the "partial_writing" option.
        </td>
    </tr>
    <tr class="row-odd">
        <td>sx, sy, sz, st</td>
        <td>int</td>
        <td>
            Partial reading / writing size of the volume to read, in voxels, in the same orientation as ox, oy, oz, ot.
        </td>
    </tr>
    <tr class="row-even">
        <td>partial_reading</td>
        <td>bool (int)</td>
        <td>Enable partial reading. Enabled by default
        </td>
    </tr>
    <tr class="row-odd">
        <td>partial_writing</td>
        <td>bool (int)</td>
        <td>Enable partial writing in an existing, already allocated file.
        </td>
    </tr>
    <tr class="row-even">
        <td>resolution_level</td>
        <td>int</td>
        <td>
            For formats allowing multi-resolution images, such as OpenSlide, this parameter specifies the resolution level to be read.
        </td>
    </tr>
    </table>

.. toctree::


// Software License Agreement (BSD License)
//
//   Stream Manipulator 3d - https://github.com/3DVision-Stack/stream-manipulator-3D
//   Copyright (c) 2016, Federico Spinelli (fspinelli@gmail.com)
//   All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder(s) nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <rqt_stream_manipulator_3d/filters/voxel_grid.h>
#include <pluginlib/class_list_macros.h>

namespace rqt_sm3d
{
namespace filters
{
    void
    VoxelGrid::init(const std::string &name)
    {
        name_ = name;
        page_= new QWidget();
        ui_.setupUi(page_);
        config = shm.segment.find<Config>((name_+"Config").c_str()).first;
        while(!config)
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(50));
            config = shm.segment.find<Config>((name_+"Config").c_str()).first;
            /* QCoreApplication::processEvents(QEventLoop::AllEvents, 100); */
        }
        {//Lock configuration mutex to read
            ShmHandler::Lock  lock(config->mtx);
            button_->setChecked(!config->disabled);
            if (!config->disabled)
                ui_.Title->setStyleSheet("background-color: green");
            ui_.downs_allB->setChecked(config->downsample_all_data);
            ui_.leafX->setValue(config->leaf_x);
            ui_.leafY->setValue(config->leaf_y);
            ui_.leafZ->setValue(config->leaf_z);
        }
        ui_.Title->setText(ui_.Title->text().prepend(name_.c_str()));
        connect(button_, SIGNAL(clicked(bool)), this, SLOT(onEnableDisable(bool)));
        connect(ui_.leafX, SIGNAL(valueChanged(double)), this, SLOT(onLeafXChanged(double)));
        connect(ui_.leafY, SIGNAL(valueChanged(double)), this, SLOT(onLeafYChanged(double)));
        connect(ui_.leafZ, SIGNAL(valueChanged(double)), this, SLOT(onLeafZChanged(double)));
        connect(ui_.downs_allB, SIGNAL(clicked(bool)), this, SLOT(onDownsampleAllData(bool)));
    }

    void
    VoxelGrid::onEnableDisable(bool checked)
    {
        if (checked)
            ui_.Title->setStyleSheet("background-color: green");
        else
            ui_.Title->setStyleSheet("background-color: red");
        ShmHandler::Lock lock(config->mtx);
        config->disabled=!checked;
    }

    void
    VoxelGrid::onDownsampleAllData(bool checked)
    {
        ShmHandler::Lock  lock(config->mtx);
        config->downsample_all_data=checked;
    }

    void
    VoxelGrid::onLeafXChanged(double val)
    {
        ShmHandler::Lock  lock(config->mtx);
        config->leaf_x = val;
    }
    void
    VoxelGrid::onLeafYChanged(double val)
    {
        ShmHandler::Lock  lock(config->mtx);
        config->leaf_y = val;
    }
    void
    VoxelGrid::onLeafZChanged(double val)
    {
        ShmHandler::Lock  lock(config->mtx);
        config->leaf_z = val;
    }
}//ns
}//ns filters

PLUGINLIB_EXPORT_CLASS(rqt_sm3d::filters::VoxelGrid, rqt_sm3d::Plugin);

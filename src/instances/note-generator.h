/*
 * Clap Validator Plugin
 *
 * A expanding set of plugins to test your host
 *
 * Copyright 2024-2025,Various authors, as described in the github
 * transaction log.
 *
 * This source repo is released under the MIT license,
 * The source code and license are at https://github.com/free-audio/clap-validator-plugin
 */

#ifndef FREEAUDIO_CVP_INSTANCES_NOTE_GENERATOR_H
#define FREEAUDIO_CVP_INSTANCES_NOTE_GENERATOR_H

#include "cvp-plugin-base.h"

namespace free_audio::cvp
{
struct NoteGeneratorPlugin : CVPClap
{
    explicit NoteGeneratorPlugin(const clap_host_t *host)
        : CVPClap(host, ValidatorFlavor::NoteGenerator)
    {
    }

  protected:
    bool startProcessing() noexcept override { return true; }
    void stopProcessing() noexcept override {}

    double priorBeat{-1234576960};
    bool isNoteOn{false};
    clap_process_status process(const clap_process *process) noexcept override
    {
        processPrecursor(process);
        if (process->transport && (process->transport->flags & CLAP_TRANSPORT_IS_PLAYING))
        {
            auto bt = 1.0 * process->transport->song_pos_beats / CLAP_BEATTIME_FACTOR;
            if (std::floor(bt) != std::floor(priorBeat))
            {
                priorBeat = bt;
                logFmt(CLAP_LOG_INFO, "NoteGenerator::process - new beat note on {}", bt);
                isNoteOn = true;
            }
            if (bt - priorBeat > 0.5 && isNoteOn)
            {
                logFmt(CLAP_LOG_INFO, "NoteGenerator::process - new half beat note off {}", bt);
                isNoteOn = false;
            }
        }
        return CLAP_PROCESS_CONTINUE;
    }
    void reset() noexcept override {}
};
} // namespace free_audio::cvp

#endif // STEREO_GENERATOR_H

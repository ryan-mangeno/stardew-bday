# Stardew Birthday Expansion

Add friends from other single-player farms, remember their birthdays, and send gifts with personal notes through the mailbox.

Mail waits in your local outbox until you and your friend are connected. Once delivered, your friend can open it later.

**Status - initial design.** C++ networking with Valve GameNetworkingSockets and C# game integration through SMAPI.

## How it connects

Proposed architecture - each player runs a SMAPI mod and a small C++ companion process. Each companion owns its local mail database.

```mermaid
flowchart TB
    subgraph A[Your computer]
        AM[Stardew and C# SMAPI mod]
        AN[C++ networking companion]
        AD[(Local inbox and outbox)]
        AM <-->|Local IPC| AN
        AN <--> AD
    end
    subgraph B[Your friend’s computer]
        BM[Stardew and C# SMAPI mod]
        BN[C++ networking companion]
        BD[(Local inbox and outbox)]
        BM <-->|Local IPC| BN
        BN <--> BD
    end
    AN <-->|GameNetworkingSockets| BN
```

Start with a direct address connection on localhost or LAN. Internet delivery needs a decision on peer authentication, signaling, and NAT traversal. Valve documents those requirements in its [P2P guide](https://github.com/ValveSoftware/GameNetworkingSockets/blob/master/README_P2P.md).

## Sending a letter

Each letter has a unique ID. A delivery confirmation means the recipient saved it locally.

```mermaid
sequenceDiagram
    participant S as Your client
    participant O as Your outbox
    participant R as Friend’s client
    participant I as Friend’s inbox
    S->>O: Save letter 123
    O-->>S: Committed
    alt Friend unavailable
        Note over S,O: Keep queued across restarts
    else Both connected
        S->>R: Send letter 123
        R->>I: Insert if ID is new
        I-->>R: Committed or already stored
        R-->>S: Confirm letter 123
        S->>O: Mark delivered
    end
```

## Disconnects and retries

```mermaid
stateDiagram-v2
    [*] --> Queued: Saved locally
    Queued --> AwaitingConfirmation: Peer connected and letter sent
    AwaitingConfirmation --> Queued: Disconnect or timeout
    AwaitingConfirmation --> Delivered: Recipient confirms storage
    AwaitingConfirmation --> Rejected: Permanent validation error
    Delivered --> [*]
```

| Case | Behavior |
| --- | --- |
| Friend offline | Letter stays queued on your computer |
| You close the game | Queue reloads next time you launch |
| Friend comes online while you are offline | Delivery waits until both clients connect |
| Confirmation gets lost | Resend the same ID; recipient confirms the existing letter |
| Recipient storage fails | Keep the letter queued until storage succeeds |
| Letter already delivered | Friend can read their local copy anytime |

**Idempotency:** receiving letter 123 repeatedly creates one inbox entry. Save the inbox entry before confirming delivery. Retry with the original ID and content.

## Birthdays

Players choose a month and day. Map March–May to Spring, June–August to Summer, September–November to Fall, and December–February to Winter. Compress each season into 28 days.

`day = floor(days_since_season_start × 28 / days_in_season) + 1`

June 1 becomes **Summer 1**. Use a fixed reference calendar so the mapping stays stable. Reminder timing, timezone, and February 29 behavior are open decisions.

## First playable milestone

- [ ] Two C++ clients exchange text letters through GameNetworkingSockets.
- [ ] Local inbox and outbox survive restarts.
- [ ] Lost confirmations and retries produce one inbox entry.
- [ ] SMAPI provides compose and mailbox screens.
- [ ] Profiles display a chosen birthday and its Stardew equivalent.

**Next - gift attachments.** Coordinate item removal and claiming with Stardew saves so crashes and day rollbacks can be reconciled. Text delivery and item ownership need separate state tracking.

## Design review

- C++ companion process or native DLL loaded by SMAPI?
- Cross Platform Considerations
- Friend identity per player or per farm?
- Birthday reminders on real dates or farm dates?
- How do friends pair and connect across the internet?
- How should gifts behave after restoring an older save?
